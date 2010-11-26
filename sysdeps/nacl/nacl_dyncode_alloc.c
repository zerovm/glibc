
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>

#include <ldsodefs.h>
#include <nacl_dyncode.h>


static char *nacl_next_code;
static char *nacl_next_data;

static size_t round_up_to_pagesize (size_t val)
{
  return (val + GLRO(dl_pagesize) - 1) & ~(GLRO(dl_pagesize) - 1);
}

static void nacl_dyncode_alloc_init (void)
{
  extern char __etext[]; /* Defined by the linker script */

  if (nacl_next_code)
    {
      return;
    }

  /* Place data after whatever brk() heap has been allocated so far. This will
     mean the brk() heap cannot be extended any further.
     TODO(mseaborn): Ideally place ld.so and brk() heap at a high address so
     that library data can be mapped below and not get in the way of the brk()
     heap.  */
  nacl_next_code = __etext;
  nacl_next_data = (char *) round_up_to_pagesize((size_t) __sbrk(0));
}

/* Allocate space for code and data simultaneously.
   This is a simple allocator that doesn't know how to deallocate.  */
void *nacl_dyncode_alloc (size_t code_size, size_t data_size,
                          size_t data_offset)
{
  assert (data_offset == round_up_to_pagesize (data_offset));

  nacl_dyncode_alloc_init ();

  code_size = round_up_to_pagesize (code_size);
  data_size = round_up_to_pagesize (data_size);

  if (data_size != 0)
    {
      size_t last_offset = nacl_next_data - nacl_next_code;
      if (data_offset > last_offset)
        {
          /* Leaves unused space in the data area. */
          nacl_next_data += data_offset - last_offset;
        }
      else if (data_offset < last_offset)
        {
          /* Leaves unused space in the code area. */
          nacl_next_code += last_offset - data_offset;
        }
      assert (nacl_next_code + data_offset == nacl_next_data);

      /* Check whether the data space is available and reserve it.
         MAP_FIXED cannot be used because it overwrites existing mappings.
         Instead, fail if returned value is different from address hint.
         TODO(mseaborn): Retry on failure or avoid failure by
         reserving a big chunk of address space at startup. */
      void *mapped = __mmap (nacl_next_data, data_size, PROT_NONE,
                             MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
      if (mapped == MAP_FAILED)
        {
          return NULL;
        }
      if (mapped != nacl_next_data)
        {
          __munmap (nacl_next_data, data_size);
          return NULL;
        }
    }

  void *code_addr = nacl_next_code;
  nacl_next_data += data_size;
  nacl_next_code += code_size;
  return code_addr;
}

void *nacl_dyncode_alloc_fixed (void *dest, size_t code_size, size_t data_size,
                                size_t data_offset)
{
  /* TODO(eaeltsin): probably these alignment requirements are overly strict.
     If really so, support unaligned case.  */
  assert (dest == round_up_to_pagesize (dest));
  assert (data_offset == round_up_to_pagesize (data_offset));

  nacl_dyncode_alloc_init ();

  if (nacl_next_code > dest)
    {
      return NULL;
    }
  nacl_next_code = dest;

  code_size = round_up_to_pagesize (code_size);
  data_size = round_up_to_pagesize (data_size);

  if (data_size != 0)
    {
      size_t last_offset = nacl_next_data - nacl_next_code;
      if (data_offset > last_offset)
        {
          /* Leaves unused space in the data area. */
          nacl_next_data += data_offset - last_offset;
        }
      else if (data_offset < last_offset)
        {
          /* Cannot move code. */
          return NULL;
        }
      assert (nacl_next_code + data_offset == nacl_next_data);

      /* Check whether the data space is available and reserve it.
         MAP_FIXED cannot be used because it overwrites existing mappings.
         Instead, fail if returned value is different from address hint.  */
      void *mapped = __mmap (nacl_next_data, data_size, PROT_NONE,
                             MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
      if (mapped == MAP_FAILED)
        {
          return NULL;
        }
      if (mapped != nacl_next_data)
        {
          __munmap (nacl_next_data, data_size);
          return NULL;
        }
    }

  nacl_next_data += data_size;
  nacl_next_code += code_size;
  return dest;
}

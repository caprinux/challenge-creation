# Initial Analysis

This is obviously a kernel challenge with all protections enabled.

The kernel module to be exploited acts as a vulnerable game server in the backend, and a simple client binary will be provided to them. Source code is also provided to the participants.

The client binary and source code does not help in the exploit, but it will help participants understand some of the kernel module functionality better.

If we notice the `run.sh` file, we can see that our kernel is booted up with 4 cores. This means that there is an opportunity to multithread, and possible exploit race conditions.

# Vulnerability 1 - Race Condition to UAF

Looking at the krpg.c code, we see that most of the functions are well protected with mutex. However, if we look at `use_health_potion()` function, we see that the mutex is not properly implemented.

For one of the cases where the user does not have a health potion but tries to use it, it does not acquire the mutex, but releases it anyways. Since this mutex is reused in other functions, we can effectively bypass this mutex by continuously running this function to release the mutex acquired by other function. 

If we look at `init_garbage_collection`, it frees items in the inventory if the refCount is less than or equals to 0. Since refCount has a `uint8_t` type, we can attempt to overflow the refCount to 0, so that it will be freed by the garbage collector, and give us a UAF.

However, there seems to be a check for refCount being more than 255. We can trivially bypass this with our race condition by trying to increase the refCount simultaneously on two threads.

# Exploitation Part 1 - Getting a leak by spraying shm_file_data

Finally, after we have managed to obtain a UAF in our equipped weapon, we want to spray a useful kernel heap object to get leaks in our kheap and kernel base. If we notice, our freed `weapon_t` struct actually has a size of 0x20, and it will be allocated into the kmalloc-32 struct.

Since all the allocations in the module is done with `GFP_KERNEL` flag without the accounting bit set, we will also need to find a kernel object that is allocated without the accounting bit and also falls within the kmalloc-32 size range.

One of the best candidates is `shm_file_data` which allows us to leak both the kernel heap and kernel object. Do note that since we are working with multiple core, and each cpu has its own slab (per-cpu slab), we will need to spray `shm_file_data` structs across all the CPUs.

After a successful spray, our weapon attack will correspond to whatever value is overwritten by the `shm_file_data` struct, allowing us to kill the dragon and unlock the feedback and reset feature.

Feedback allows us to allocate a kheap chunk of any size `N > 32` in the accounting slab once. Subsequently, it will store the size allocated into a struct, and if we want to provide more feedback, we will be limited by the provided size.

# Vulnerability 2 - Array negative indexing

Since the pointer to feedback and the pointer to mobs is beside each other in memory, we can simply RESET until `cur_mob` is -2, allowing us to treat the FEEDBACK as a mob. This allows us to change the size in the feedback struct, and gives us a heap overflow when we provide more feedback later on.

TODO: (tty_struct does not work since it goes into the accounting slab)

Using the heap overflow, we can spray another struct `tty_struct`, and allow us to overwrite the vtable of operations to any arbitrary controlled address. From here, we can pivot and write a kernel rop chain to commit root credentials and give us a root shell.

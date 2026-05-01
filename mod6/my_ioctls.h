#define MY_HW_READ 	_IOR('z', 123, unsigned int *)
#define MY_HW_WRITE	_IOW('z', 124, unsigned int *)
/***************************************************************************************/
/*
 * #define MY_HW_READ 	_IOR('z', 123, unsigned int *)
 * MY_HW_READ:
 * - IOCTL command to READ data from kernel space to user space
 * - '_IOR' means: IOCTL Read (Kernel → User)
 *
 * Parameters:
 * 'z'   → Magic number (unique identifier for this driver)
 * 123   → Command number (unique inside this driver)
 * unsigned int * → Data type that will be transferred
 */
/***************************************************************************************/
 /* #define MY_HW_WRITE	_IOW('z', 124, unsigned int *)
 * MY_HW_WRITE:
 * - IOCTL command to WRITE data from user space to kernel space
 * - '_IOW' means: IOCTL Write (User → Kernel)
 *
 * Parameters:
 * 'z'   → Magic number (unique identifier for this driver)
 * 124   → Command number (unique inside this driver)
 * unsigned int * → Data type passed from user space
 */
/***************************************************************************************/











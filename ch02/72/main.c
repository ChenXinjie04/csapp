/* Copy integer into buffer if space is available */
/* WARNING: The following code is buggy */
void copy_int(int val, void *buf, int maxbytes) {
    /* If use maxbytes - sizeof(val) as conditional test the result will be unsigned int,
    which is always nonzero. */
    if (maxbytes >= sizeof(val)) {
        memcpy(buf, (void *) &val, sizeof(val));
    }
}
/**@brief ����linux kfifoд��ring buffer
 *@atuher Anker  date:2013-12-18
* ring_buffer.h
 * */

#ifndef KFIFO_HEADER_H 
#define KFIFO_HEADER_H

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

//�ж�x�Ƿ���2�Ĵη�
#define is_power_of_2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))
//ȡa��b����Сֵ
#define min(a, b) (((a) < (b)) ? (a) : (b))

struct ring_buffer
{
    void         *buffer;     //������
    unsigned int     size;       //��С
    unsigned int     in;         //���λ��
    unsigned int       out;        //����λ��
    pthread_spinlock_t *f_lock;    //������
};
//��ʼ��������
static __inline struct ring_buffer* ring_buffer_init(void *buffer, unsigned int size, pthread_spin_t *f_lock)
{
    assert(buffer);
    struct ring_buffer *ring_buf = NULL;
	
    if (!is_power_of_2(size))
    {
    	fprintf(stderr,"size must be power of 2.\n");
        return ring_buf;
    }
    ring_buf = (struct ring_buffer *)malloc(sizeof(struct ring_buffer));
    if (!ring_buf)
    {
        fprintf(stderr,"Failed to malloc memory,errno:%u,reason:%s",
            errno, strerror(errno));
        return ring_buf;
    }
    memset(ring_buf, 0, sizeof(struct ring_buffer));
    ring_buf->buffer = buffer;
    ring_buf->size = size;
    ring_buf->in = 0;
    ring_buf->out = 0;
    ring_buf->f_lock = f_lock;
    return ring_buf;
}
//�ͷŻ�����
static __inline void ring_buffer_free(struct ring_buffer *ring_buf)
{
    if (ring_buf)
    {
	    if (ring_buf->buffer)
	    {
	        free(ring_buf->buffer);
	        ring_buf->buffer = NULL;
	    }
		pthread_spin_destroy(ring_buf->f_lock);
		
	    free(ring_buf);
	    ring_buf = NULL;
    }
}

//�������ĳ���
static __inline unsigned int __ring_buffer_len(const struct ring_buffer *ring_buf)
{
    return (ring_buf->in - ring_buf->out);
}

//�ӻ�������ȡ����
static __inline unsigned int __ring_buffer_get(struct ring_buffer *ring_buf, void * buffer, unsigned int size)
{
    assert(ring_buf || buffer);
    unsigned int len = 0;
    size  = min(size, ring_buf->in - ring_buf->out);        
    /* first get the data from fifo->out until the end of the buffer */
    len = min(size, ring_buf->size - (ring_buf->out & (ring_buf->size - 1)));
    memcpy(buffer, ring_buf->buffer + (ring_buf->out & (ring_buf->size - 1)), len);
    /* then get the rest (if any) from the beginning of the buffer */
    memcpy(buffer + len, ring_buf->buffer, size - len);
    ring_buf->out += size;
    return size;
}

//�򻺳����д������
static __inline unsigned int __ring_buffer_put(struct ring_buffer *ring_buf, void *buffer, unsigned int size)
{
    assert(ring_buf || buffer);
    unsigned int len = 0;//256    270
	
    size = min(size, ring_buf->size - ring_buf->in + ring_buf->out);
    /* first put the data starting from fifo->in to buffer end ((ring_buf->in & (ring_buf->size - 1)���in��ʵ�ʻ����е�λ��)*/
    len  = min(size, ring_buf->size - (ring_buf->in & (ring_buf->size - 1)));
    memcpy(ring_buf->buffer + (ring_buf->in & (ring_buf->size - 1)), buffer, len);
    /* then put the rest (if any) at the beginning of the buffer */
    memcpy(ring_buf->buffer, buffer + len, size - len);
    ring_buf->in += size;
	
    return size;
}

//��ȡ����--------------------------------------------------------------
static __inline unsigned int ring_buffer_len(const struct ring_buffer *ring_buf)
{
    unsigned int len = 0;
    pthread_spin_lock(ring_buf->f_lock);
    len = __ring_buffer_len(ring_buf);
    pthread_spin_unlock(ring_buf->f_lock);
    return len;
}
//��ȡ����--------------------------------------------------------------
static __inline unsigned int ring_buffer_get(struct ring_buffer *ring_buf, void *buffer, unsigned int size)
{
    unsigned int ret;
    pthread_spin_lock(ring_buf->f_lock);
    ret = __ring_buffer_get(ring_buf, buffer, size);
    //buffer��û������
    if (ring_buf->in == ring_buf->out)
    ring_buf->in = ring_buf->out = 0;
    pthread_spin_unlock(ring_buf->f_lock);
    return ret;
}
//�������--------------------------------------------------------------
static __inline unsigned int ring_buffer_put(struct ring_buffer *ring_buf, void *buffer, unsigned int size)
{
    unsigned int ret;
    pthread_spin_lock(ring_buf->f_lock);
    ret = __ring_buffer_put(ring_buf, buffer, size);
    pthread_spin_unlock(ring_buf->f_lock);
    return ret;
}
#endif


#undef NDEBUG
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lcthw/dbg.h>
#include <lcthw/ringbuffer.h>

RingBuffer *RingBuffer_create(int length) {
	RingBuffer *buffer = calloc(1, sizeof(RingBuffer));
	buffer->length = length + 1;
	buffer->start = 0;
	buffer->end = 0;
	buffer->buffer = calloc(buffer->length, 1);

	return buffer;
}

void RingBuffer_destroy(RingBuffer *buffer) {
	if (buffer) {
		free(buffer->buffer);
		free(buffer);
	}
}

int RingBuffer_write(RingBuffer *buffer, char *data, int length) {
	if (RingBuffer_available_data(buffer) == 0) {
		buffer->start = buffer->end = 0;
	}

	check (length <= RingBuffer_available_space(buffer), 
			"not enough space: %d request, %d available",
			RingBuffer_available_data(buffer), length);

	void *result = memcpy(RingBuffer_ends_at(buffer), data, length);

	check(result != NULL, "failed to write data into buffer");

	RingBuffer_commit_write(buffer, length);
	return length;
error:
	return -1;
}

int RingBuffer_read(RingBuffer *buffer, char *target, int amount) {
	check_debug(amount <= RingBuffer_available_data(buffer),
				"not enough in the buffer: has %d, needs %d",
			RingBuffer_available_data(buffer), amount);

	void *result = memcpy(target, RingBuffer_starts_at(buffer), amount);

	check(result != NULL, "failed to write buffer into data");

	RingBuffer_commit_read(buffer, amount);

	if (buffer->end == buffer->start) {
		buffer->start = buffer->end = 0;
	}

	return amount;
error:
	return -1;
}

bstring RingBuffer_gets(RingBuffer *buffer, int amount) {
	check(amount > 0, "need more than 0 for gets, you gave: %d", amount);
	check_debug(amount <= RingBuffer_available_data(buffer), "not enough in the buffer");

	bstring result = blk2bstr(RingBuffer_starts_at(buffer), amount);

	check(result != NULL, "failed to create gets result");
	check(blength(result) == amount, "wrong result length");
	
	RingBuffer_commit_read(buffer, amount);
	assert(RingBuffer_available_data(buffer) >= 0 && "error in read commit");

	return result;
error:
	return NULL;
}

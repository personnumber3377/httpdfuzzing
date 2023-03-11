#include <stddef.h>
#include "aux_new.h"
#include "custom_mutator_helpers.h"
#include "afl-fuzz.h"

typedef struct my_mutator {

	afl_state_t *afl;

	// any additional data here!
	size_t trim_size_current;
	int    trimmming_steps;
	int    cur_step;

	int brute_level;

	int byte_count_1;
	int byte_count_2;

	int index_i;
	int index_j;

	int i;
	int j;
	int k;
	int l;

	int dict3_size;
	int pending_dict3;

	size_t dict4_size;
	size_t pending_dict4;
	u8 *dict4;
	u_int n;

	u_int num_lines;

	u8 havoc_max_mult_OLD;

	int max_steps;

	STAGE stage;

	//generator
	int remaining;
	int new_level;

	//incremental
	int next_level;
	int cur_length;


	// Reused buffers:
	BUF_VAR(u8, fuzz);
	BUF_VAR(u8, data);
	BUF_VAR(u8, havoc);
	BUF_VAR(u8, trim);
	BUF_VAR(u8, post_process);

} my_mutator_t;


/*

notes:

ok so we know that maybe_grow returns a pointer to the grown buffer.




#define BUF_VAR(type, name) \
  type * name##_buf;        \
  size_t name##_size;


 this filles in `&structptr->something_buf, &structptr->something_size`.


#define BUF_PARAMS(struct, name) \
  (void **)&struct->name##_buf, &struct->name##_size

*/


size_t lines_mix(my_mutator_t *data, uint8_t *buf, size_t buf_size,
        u8 **out_buf, size_t max_size, char separator){

	size_t mutated_size = 10000;
	// u8 *mutated_out = maybe_grow(BUF_PARAMS(data, fuzz), mutated_size);


	//u8 *mutated_out = 1;

	u8 *mutated_out = maybe_grow(&data->fuzz_buf, &data->fuzz_size, mutated_size);


	//Initial stage
	if(data->stage == 0){
		//printf("Bullshit %u\n", data->afl->pending_not_fuzzed);
		if(data->afl->pending_not_fuzzed == 0){
			data->stage++;
			//data->remaining = data->afl->queued_paths;
			data->remaining = data->afl->queued_items;

		}else{

			mutated_size = 0;

			if(data->afl->queue_cur->was_fuzzed){
				goto end;
			}

			data->afl->stage_max = 1;

			u_int ptr = 0;
			for(int i=0; i<buf_size; i++){

				if(buf[i] == separator){

					u_int length = i-ptr;

					if(length > 0){

						if(!repeated(data, &buf[ptr], length)){

							lines[data->num_lines].p = malloc(length);
							memcpy(lines[data->num_lines].p, &buf[ptr], length);

							lines[data->num_lines].size = length;

							data->num_lines++;
						}
					}
					ptr = i+1;
				}
			}
		}
	}

	//Line-mix stage
	if(data->stage == 1){

		if(data->afl->queue_cur->was_fuzzed == 0){
			if(data->afl->stage_cur == 0)
				data->remaining++;
			return brute_8bits(data, buf, buf_size, out_buf, max_size);
		}

		if(data->afl->queue_cur->trim_done > 0){
			mutated_size = 0;
			data->afl->stage_max = 1;
			goto end;
		}

		u_int n_lines;

		if(data->afl->stage_cur == 0){

			n_lines = 1;
			for(int i=0; i<buf_size; i++){
				if(buf[i] == separator){
					n_lines++;
				}
			}

			data->max_steps = n_lines * data->num_lines;

			data->havoc_max_mult_OLD = data->afl->havoc_max_mult;
			data->index_i = 0;
			data->index_j = 0;
		}

		data->afl->stage_max = data->max_steps;
		data->afl->havoc_max_mult = 0;

		mutated_size = buf_size <= max_size ? buf_size : max_size;

		// |0|1|2|3|4|5|

		uint ptr = data->index_i;

		while(ptr < buf_size && buf[ptr] != separator){
			ptr++;
		}

		uint post_size = buf_size - ptr;

		uint new_size = data->index_i + lines[data->index_j].size + post_size;
		uint8_t *new_array = malloc(new_size);

		uint tmp = 0;

		if(data->index_i){
			memcpy(&new_array[tmp], &buf[0], data->index_i);
			tmp += data->index_i;
		}

		memcpy(&new_array[tmp], lines[data->index_j].p, lines[data->index_j].size);
		tmp += lines[data->index_j].size;

		if(post_size){
			memcpy(&new_array[tmp], &buf[ptr], post_size);
		}

		//printf("%d %d\n", new_size, mutated_size);
		if(new_size > mutated_size){
			mutated_size = new_size;
		}
		/*
		printf("%s", "Now trying the shit.\n");
		printf("(void**)&buf : %x\n", (void**)&buf);
		printf("(void**)buf : %x\n", (void**)buf);
		printf("&buf_size = %x\n", &buf_size);
		printf("buf_size = %x\n", buf_size);
		printf("mutated_size : %x\n", mutated_size);

		printf("buffer contents: %s", buf);
		*/


		//mutated_out = maybe_grow((void**)&buf, &buf_size, mutated_size);
		
		/*
		Here the original programmer made a mistake, because buf was not actually malloc'ed anywhere. Instead we need to use something similar to:

		maybe_grow(BUF_PARAMS(data, trim), buf_size)

		as found in the example.c file.


		




		*/

		*mutated_out = maybe_grow(&data->fuzz_buf, &data->fuzz_size, mutated_size);
		//printf("mutated_out : %x\n", mutated_out);

		if (!mutated_out) {
			*out_buf = NULL;
			perror("custom mutator allocation (maybe_grow)");
			return 0;
		}

		mutated_size = new_size;

		memcpy(mutated_out, new_array, new_size);
		//printf("mutated_out : %s\n", mutated_out);
		//printf("mutated_out: ");
		//write(1, mutated_out, new_size);

		free(new_array);

		data->index_j++;

		if(data->index_j >= data->num_lines){
			data->index_j = 0;
			data->index_i = ptr+1;
		}

		if(data->afl->stage_cur == data->afl->stage_max-1){
			data->afl->queue_cur->trim_done = 1;
			data->remaining--;
			data->afl->havoc_max_mult = data->havoc_max_mult_OLD;
		}
	}

	end:
	*out_buf = mutated_out;
	//printf("output:\n");
	//write(1, **out_buf, mutated_size);

	//printf("%s\n", *out_buf);
	//printf("input: \n");
	//write(1, buf, buf_size);
	return mutated_size;
}


size_t afl_custom_fuzz(my_mutator_t *data, uint8_t *buf, size_t buf_size,
                       u8 **out_buf, uint8_t *add_buf, size_t add_buf_size,
                       size_t max_size) {


		size_t tmp = lines_mix(data, buf, buf_size, out_buf, max_size, 0x0A);
		return tmp;

}





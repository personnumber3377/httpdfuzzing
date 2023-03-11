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

size_t inputs_n_bytes(size_t size, my_mutator_t *data){

	if(size == 0)
		return 1;

	size_t num = 0;

	//struct queue_entry * current = data->afl->queue;
	struct queue_entry * current = data->afl->queue_buf[0];

	//while(current != 0){
	for (int i=0; i<data->afl->queued_items; i++){
		if(current->len == size){
			num++;
		}
		current = data->afl->queue_buf[i];
	}
	return num;
}

void set_all_not_fuzzed(my_mutator_t *data){
	int count = 0;
	data->afl->pending_not_fuzzed = data->afl->queued_items;

	struct queue_entry * current = data->afl->queue_buf[count];

	//while(current != data->afl->queue_top){
	// for(int i=0; i<data->afl->queued_items; i++){
	for (int i = 0; i<data->afl->queued_items; i++) {
		//count++;
		current->was_fuzzed = 0;
		current = data->afl->queue_buf[i];
	}
}

void reset_pending(my_mutator_t *data){

	data->afl->pending_not_fuzzed = data->afl->queued_items;
}

int repeated(my_mutator_t *data, void *line, uint length){

	int rep = 0;

	for(int i=0; i<data->num_lines; i++){
		if(lines[i].size == length){
			if(memcmp(lines[i].p, line, length) == 0){
				rep = 1;
				break;
			}
		}

	}

	return rep;
}


my_mutator_t *afl_custom_init(afl_state_t *afl, unsigned int seed) {

  srand(seed);

  my_mutator_t *data = calloc(1, sizeof(my_mutator_t));
  if (!data) {

    perror("afl_custom_init alloc");
    return NULL;

  }

  data->brute_level = 0;

  char *level;
  if (( level =getenv("BRUTE_LEVEL")) != NULL )
	  data->brute_level = strtol(level, NULL, 10);


  
  if ((data->fuzz_buf = malloc(MAX_FILE)) == NULL) {

    perror("mutator_buf alloc");
    return NULL;

  }
  

  data->stage = BRUTE;

  data->cur_step = 0;

  data->index_i = 0;
  data->index_j = 0;

  data->i = 0;
  data->j = 0;
  data->k = 0;
  data->l = 0;

  data->dict3_size = 0;
  data->dict4_size = 0;

  data->dict4 = NULL;
  data->n = 0;

  data->num_lines = 0;

  data->byte_count_1 = 0;
  data->byte_count_2 = 0;

  data->afl = afl;

  //generator
  data->remaining = 0;
  data->new_level = 1;

  //incremental
  data->next_level = 0;
  data->cur_length = 0;

  //data->seed = seed;

  //radamsa_init();

  return data;

}

void afl_custom_deinit(my_mutator_t *data) {

	  free(data->post_process_buf);
	  free(data->havoc_buf);
	  free(data->data_buf);
	  free(data->fuzz_buf);
	  free(data->trim_buf);
	  free(data);

}

void reset_values(my_mutator_t *data, int new_length){

	//struct queue_entry * ptr = data->afl->queue;
	struct queue_entry * ptr = data->afl->queue_buf[0];

	int fd;



	/*
	The proper way to do this is like this in the new afl++ :

	for (idx = 0; idx < afl->queued_items; idx++) {

      if (afl->queue_buf[idx] && !afl->queue_buf[idx]->disabled &&
          afl->queue_buf[idx]->depth > afl->max_depth)
        afl->max_depth = afl->queue_buf[idx]->depth;

    }

	*/
	for(int i=0; i<data->afl->queued_items; i++){

		//ptr->was_fuzzed = 0;
		ptr->trim_done = 1;

		ptr->len = new_length;

		ptr = data->afl->queue_buf[i];
	}


	data->remaining = data->afl->queued_items;



}

void zero_add(my_mutator_t *data){

	//struct queue_entry * ptr = data->afl->queue;
	struct queue_entry * ptr = data->afl->queue_buf[0];

	int fd;
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	u8 null = 0;

	for(int i=0; i<data->afl->queued_items; i++){

		fd = open(ptr->fname, O_WRONLY|O_APPEND, mode);
		if(fd){
			write(fd, &null, 1);
			close(fd);
		}

		ptr = data->afl->queue_buf[i];
	}

}


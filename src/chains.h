/** @file
 * A chain is a linked list of blocks.  These chain functions
 * support managing chains.
 */

#ifndef CHAINS_H
#define CHAINS_H


typedef bool (*chain_walker)(DB_HANDLE *handle, HEAD_HANDLE *head_handle, void *closure);

RECNO_ERROR chains_chain_walk(DB_HANDLE *handle,
                              HEAD_HANDLE *head_chain,
                              chain_walker walker,
                              void *closure);

RECNO_ERROR chains_chain_augment(DB_HANDLE *handle,
                                 HEAD_HANDLE *head_chain,
                                 size_t min_length,
                                 HEAD_HANDLE *new_block);




#endif

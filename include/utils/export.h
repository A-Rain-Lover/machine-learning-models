#ifndef _EXPORT_H_
#define _EXPORT_H_

#include <stddef.h>
#include <stdint.h>

typedef struct dataset dataset_t;
typedef struct labeled_dataset labeled_dataset_t;
typedef struct history_entry history_t;
typedef struct neuron neuron_t;
typedef struct single_layer_model single_layer_model_t;

uint8_t gp_export_dataset(dataset_t *dataset, const char *filename);

uint8_t gp_export_labeled_dataset(labeled_dataset_t *dataset,
                                  const char *filename);

uint8_t gp_export_history(history_t *history, const char *filename);

uint8_t gp_export_weights(neuron_t *neuron, const char *filename);
uint8_t gp_export_single_layer_model(single_layer_model_t *model,
                                     history_t *history);
#endif
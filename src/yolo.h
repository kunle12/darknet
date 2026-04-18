#ifndef YOLO_H
#define YOLO_H

#include "box.h"
#include "cost_layer.h"
#include "demo.h"
#include "detection_layer.h"
#include "network.h"
#include "parser.h"
#include "utils.h"

#ifdef OPENCV
#include "opencv2/highgui/highgui_c.h"
#endif

void train_yolo(char *cfgfile, char *weightfile);
void convert_yolo_detections(float *predictions, int classes, int num,
                             int square, int side, int w, int h, float thresh,
                             float **probs, box *boxes, int only_objectness);
void validate_yolo(char *cfgfile, char *weightfile);
void validate_yolo_recall(char *cfgfile, char *weightfile);

#endif // YOLO_H

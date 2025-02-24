#ifndef _SAVE_H
#define _SAVE_H 1

#include "shared.h"

typedef enum {
  SAVE_RESULT_OK,
  SAVE_RESULT_ERROR,
  SAVE_RESULT_CONFLICT
} SaveResult;

SaveResult save_device(const string name, const string mac_addr);
SaveResult delete_saved_device(const string name);
SaveResult modify_saved_device(const string name, const string new_mac_addr);
string resolve_saved_device(const string name);
string* list_saved_devices(void);

#endif // _SAVE_H

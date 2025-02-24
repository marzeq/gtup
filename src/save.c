#include "save.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

string strappend(const string s1, const string s2) {
  size_t len1 = strlen(s1);
  size_t len2 = strlen(s2);
  string result = malloc(len1 + len2 + 1);
  if (!result)
    return NULL;
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

string get_save_path(void) {
  const string home = getenv("HOME");
  if (home == NULL) {
    return "";
  }

  const string suffix = "/.local/state/gtup";
  return strappend(home, suffix);
}

int create_directory_if_not_exists(const string path) {
  struct stat st = {0};
  if (stat(path, &st) == -1) {
    if (mkdir(path, 0700) == -1) {
      return -1;
    }
  }
  return 0;
}

#define SAVE_FILE strappend(get_save_path(), "/devices")

SaveResult save_device(const string name, const string mac_addr) {
  const string save_path = get_save_path();
  if (create_directory_if_not_exists(save_path) == -1) {
    return SAVE_RESULT_ERROR;
  }

  FILE* f = fopen(SAVE_FILE, "r");
  if (f) {
    char line[256];
    while (fgets(line, sizeof(line), f)) {
      char saved_name[128], saved_mac[128];
      sscanf(line, "%127[^:]:%127s", saved_name, saved_mac);
      if (strcmp(saved_name, name) == 0) {
        fclose(f);
        return SAVE_RESULT_CONFLICT;
      }
    }
    fclose(f);
  }

  f = fopen(SAVE_FILE, "a");
  if (f == NULL) {
    return SAVE_RESULT_ERROR;
  }

  fprintf(f, "%s:%s\n", name, mac_addr);
  fclose(f);
  return SAVE_RESULT_OK;
}

SaveResult delete_saved_device(const string name) {
  const string save_path = get_save_path();
  if (create_directory_if_not_exists(save_path) == -1) {
    return SAVE_RESULT_ERROR;
  }

  const string temp_file = strappend(save_path, "/devices.tmp");
  FILE* f = fopen(SAVE_FILE, "r");
  FILE* temp = fopen(temp_file, "w");
  if (!f || !temp) {
    return SAVE_RESULT_ERROR;
  }

  char line[256];
  while (fgets(line, sizeof(line), f)) {
    char saved_name[128], saved_mac[128];
    sscanf(line, "%127[^:]:%127s", saved_name, saved_mac);
    if (strcmp(saved_name, name) != 0) {
      fprintf(temp, "%s:%s\n", saved_name, saved_mac);
    }
  }

  fclose(f);
  fclose(temp);
  remove(SAVE_FILE);
  rename(temp_file, SAVE_FILE);
  return SAVE_RESULT_OK;
}

SaveResult modify_saved_device(const string name, const string new_mac_addr) {
  const string save_path = get_save_path();
  if (create_directory_if_not_exists(save_path) == -1) {
    return SAVE_RESULT_ERROR;
  }

  const string temp_file = strappend(save_path, "/devices.tmp");
  FILE* f = fopen(SAVE_FILE, "r");
  FILE* temp = fopen(temp_file, "w");
  if (!f || !temp) {
    return SAVE_RESULT_ERROR;
  }

  char line[256];
  while (fgets(line, sizeof(line), f)) {
    char saved_name[128], saved_mac[128];
    sscanf(line, "%127[^:]:%127s", saved_name, saved_mac);
    if (strcmp(saved_name, name) == 0) {
      fprintf(temp, "%s:%s\n", name, new_mac_addr);
    } else {
      fprintf(temp, "%s:%s\n", saved_name, saved_mac);
    }
  }

  fclose(f);
  fclose(temp);
  remove(SAVE_FILE);
  rename(temp_file, SAVE_FILE);
  return SAVE_RESULT_OK;
}

string resolve_saved_device(const string name) {
  FILE* f = fopen(SAVE_FILE, "r");
  if (!f) {
    return NULL;
  }

  char line[256];
  while (fgets(line, sizeof(line), f)) {
    char saved_name[128], saved_mac[128];
    sscanf(line, "%127[^:]:%127s", saved_name, saved_mac);
    if (strcmp(saved_name, name) == 0) {
      fclose(f);
      return strdup(saved_mac);
    }
  }

  fclose(f);
  return NULL;
}

#include "shell.h"
#include "kernel.h"
#include "std_lib.h"
#include "filesystem.h"

void shell() {
  char buf[64];
  char cmd[64];
  char arg[2][64];

  byte cwd = FS_NODE_P_ROOT;

  while (true) {
    printString("MengOS:");
    printCWD(cwd);
    printString("$ ");
    readString(buf);
    parseCommand(buf, cmd, arg);

    if (strcmp(cmd, "cd")) cd(&cwd, arg[0]);
    else if (strcmp(cmd, "ls")) ls(cwd, arg[0]);
    else if (strcmp(cmd, "mv")) mv(cwd, arg[0], arg[1]);
    else if (strcmp(cmd, "cp")) cp(cwd, arg[0], arg[1]);
    else if (strcmp(cmd, "cat")) cat(cwd, arg[0]);
    else if (strcmp(cmd, "mkdir")) mkdir(cwd, arg[0]);
    else if (strcmp(cmd, "clear")) clearScreen();
    else printString("Invalid command\n");
  }
}

// TODO: 4. Implement printCWD function
void printCWD(byte cwd) {
  struct node_fs node_fs_buf;
  char path[FS_MAX_NODE][MAX_FILENAME];
  int depth = 0;
  byte temp;
  int i;

  readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  if (cwd == FS_NODE_P_ROOT)
  {
    printString("/");
    return;
  }

  while (cwd != FS_NODE_P_ROOT)
  {
    temp = cwd;
    strcpy(path[depth], node_fs_buf.nodes[temp].node_name);
    cwd = node_fs_buf.nodes[temp].parent_index;
    depth++;
  }

  // printString("/");

  for (i = depth - 1; i >= 0; i--)
  {
    printString("/");
    printString(path[i]);
    // if (i == 0)
    // {
    //   printString("/");
    // }
  }
}

// TODO: 5. Implement parseCommand function
void parseCommand(char* buf, char* cmd, char arg[2][64]) {
  int i = 0, j = 0, arg_num = 0;

  int k;
  
  while (buf[i] == ' ') i++;
  
  while (buf[i] != ' ' && buf[i] != '\0') {
      cmd[j++] = buf[i++];
  }
  cmd[j] = '\0';
  
  while (buf[i] != '\0') {
      if (buf[i] == ' ') {
          i++;
          j = 0;
          while (buf[i] != ' ' && buf[i] != '\0') {
              arg[arg_num][j++] = buf[i++];
          }
          arg[arg_num][j] = '\0';
          arg_num++;
          if (arg_num >= 2) break;
      } else {
          i++;
      }
  }
  
  
  
  for (k = arg_num; k < 2; k++) {
      arg[k][0] = '\0';
  }
}

// TODO: 6. Implement cd function
void cd(byte *cwd, char *dirname) {
  struct node_fs node_fs_buf;
  int i;
  bool found = false;

  readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  if (strcmp(dirname, "..") == 1) {
      if (*cwd != FS_NODE_P_ROOT) {
          *cwd = node_fs_buf.nodes[*cwd].parent_index;
      }
      return;
  }

  if (strcmp(dirname, "/") == 1) {
      *cwd = FS_NODE_P_ROOT;
      return;
  }

  for (i = 0; i < FS_MAX_NODE; i++) {
      if (node_fs_buf.nodes[i].parent_index != *cwd) continue;
      if (!strcmp(node_fs_buf.nodes[i].node_name, dirname)) continue;
      
      if (node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR) {
          *cwd = i;
          found = true;
          break;
      }
  }

  if (!found) {
      printString("cd: ");
      printString(dirname);
      printString(": Directory not found\n");
  }
}


// TODO: 7. Implement ls function
void ls(byte cwd, char *dirname) {
  struct node_fs node_fs_buf;
  int i;
  byte target_dir = cwd;
  bool found = false;

  readSector((byte*)&node_fs_buf, FS_NODE_SECTOR_NUMBER);
  readSector((byte*)&node_fs_buf + SECTOR_SIZE, FS_NODE_SECTOR_NUMBER + 1);

  if (dirname[0] != '\0' && strcmp(dirname, ".") != 0) {
    found = false;
    for (i = 0; i < FS_MAX_NODE; i++) {
      if (node_fs_buf.nodes[i].parent_index == cwd &&
          strcmp(node_fs_buf.nodes[i].node_name, dirname) == 0 &&
          node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR) {
        target_dir = i;
        found = true;
        break;
      }
    }
    
    if (!found) {
      printString("Directory not found\n");
      return;
    }
  }

  // Tampilkan isi direktori
  found = false;
  for (i = 0; i < FS_MAX_NODE; i++) {
    if (node_fs_buf.nodes[i].parent_index == target_dir) {
      printString(node_fs_buf.nodes[i].node_name);
      printString(" ");
      if (node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR) {
        printString(" ");
      }
      found = true;
    }
  }
  printString("\n");

  if (!found) {
    printString("Empty directory\n");
  }
}

// TODO: 8. Implement mv function
void mv(byte cwd, char* src, char* dst) {
  struct node_fs node_buf;
  int i, src_index = -1;
  byte dst_parent_index = cwd;
  char output_name[MAX_FILENAME];
  bool dst_is_dir = false;

  readSector(&node_buf.nodes[0], FS_NODE_SECTOR_NUMBER);
  readSector(&node_buf.nodes[32], FS_NODE_SECTOR_NUMBER + 1);

  for (i = 0; i < FS_MAX_NODE; i++) {
    if (node_buf.nodes[i].parent_index == cwd &&
        node_buf.nodes[i].node_name[0] != '\0' &&
        node_buf.nodes[i].data_index != FS_NODE_D_DIR &&
        strcmp(node_buf.nodes[i].node_name, src) == 1) {
      src_index = i;
      break;
    }
  }

  if (src_index == -1) {
    printString("mv: source file not found\n");
    return;
  }

  if (dst[0] == '/' && dst[1] != '\0') {
    dst_parent_index = FS_NODE_P_ROOT;
    strcpy(output_name, dst + 1);
  } else if (dst[0] == '.' && dst[1] == '.' && dst[2] == '/' && dst[3] != '\0') {
    if (cwd == FS_NODE_P_ROOT) {
      printString("mv: already at root directory\n");
      return;
    }
    dst_parent_index = node_buf.nodes[cwd].parent_index;
    strcpy(output_name, dst + 3);
  } else {
    int slash_pos = -1;
    for (i = 0; dst[i] != '\0'; i++) {
      if (dst[i] == '/') {
        slash_pos = i;
        break;
      }
    }

    if (slash_pos != -1) {
      char dirname[MAX_FILENAME];
      for (i = 0; i < slash_pos; i++) dirname[i] = dst[i];
      dirname[slash_pos] = '\0';

      for (i = 0; i < FS_MAX_NODE; i++) {
        if (node_buf.nodes[i].parent_index == cwd &&
            strcmp(node_buf.nodes[i].node_name, dirname) == 1 &&
            node_buf.nodes[i].data_index == FS_NODE_D_DIR) {
          dst_parent_index = i;
          strcpy(output_name, dst + slash_pos + 1);
          break;
        }
      }
    } else {
      for (i = 0; i < FS_MAX_NODE; i++) {
        if (node_buf.nodes[i].parent_index == cwd &&
            strcmp(node_buf.nodes[i].node_name, dst) == 1 &&
            node_buf.nodes[i].data_index == FS_NODE_D_DIR) {
          dst_parent_index = i;
          strcpy(output_name, src);
          dst_is_dir = true;
          break;
        }
      }

      if (!dst_is_dir) {
        strcpy(output_name, dst);
      }
    }
  }

  if (strlen(output_name) == 0 || strlen(output_name) > 13) {
    printString("mv: invalid destination filename\n");
    return;
  }

  for (i = 0; i < FS_MAX_NODE; i++) {
    if (i != src_index &&
        node_buf.nodes[i].parent_index == dst_parent_index &&
        strcmp(node_buf.nodes[i].node_name, output_name) == 1) {
      printString("mv: destination file exists\n");
      return;
    }
  }

  node_buf.nodes[src_index].parent_index = dst_parent_index;
  strcpy(node_buf.nodes[src_index].node_name, output_name);

  writeSector(&node_buf.nodes[0], FS_NODE_SECTOR_NUMBER);
  writeSector(&node_buf.nodes[32], FS_NODE_SECTOR_NUMBER + 1);
}


// TODO: 9. Implement cp function
void cp(byte cwd, char *src, char *dst) {
  struct node_fs node_fs_buf;
  struct file_metadata src_meta, dst_meta;
  enum fs_return status;
  byte dst_parent_index = cwd;
  int i, j, slash_idx = -1, found = 0;
  char foldername[14], dstname[14];

  for (i = 0; i < 14; i++) {
    foldername[i] = 0;
    dstname[i] = 0;
  }

  readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  src_meta.parent_index = cwd;
  strcpy(src_meta.node_name, src);
  fsRead(&src_meta, &status);

  if (status != FS_SUCCESS) {
    printString("cp: file not found\n");
    return;
  }

  if (src_meta.filesize == 0) {
    printString("cp: cannot copy a directory\n");
    return;
  }

  if (dst[0] == '/' && dst[1] != '\0') {
    dst_parent_index = FS_NODE_P_ROOT;
    strcpy(dstname, dst + 1);
  } 
  
  else if (dst[0] == '.' && dst[1] == '.' && dst[2] == '/') {
    dst_parent_index = node_fs_buf.nodes[cwd].parent_index;
    strcpy(dstname, dst + 3);
  } 
  
  else {
    for (i = 0; dst[i] != '\0'; i++) {
      if (dst[i] == '/') {
        slash_idx = i;
        break;
      }
    }

    if (slash_idx != -1) {
      for (j = 0; j < slash_idx && j < 13; j++) {
        foldername[j] = dst[j];
      }

      foldername[j] = '\0';
      strcpy(dstname, dst + slash_idx + 1);

      found = 0;
      for (i = 0; i < FS_MAX_NODE; i++) {
        if (node_fs_buf.nodes[i].parent_index == cwd &&
            strcmp(node_fs_buf.nodes[i].node_name, foldername) == 1 &&
            node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR) {
          dst_parent_index = i;
          found = 1;
          break;
        }
      }

      if (!found) {
        strcpy(dstname, dst);
        dst_parent_index = cwd;
      }
    } 
    else {
      found = 0;
      for (i = 0; i < FS_MAX_NODE; i++) {
        if (node_fs_buf.nodes[i].parent_index == cwd && strcmp(node_fs_buf.nodes[i].node_name, dst) == 1 && node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR) {
          dst_parent_index = i;
          strcpy(dstname, src);
          found = 1;
          break;
        }
      }

      if (!found) strcpy(dstname, dst);
      
    }
  }

  if (strlen(dstname) == 0 || strlen(dstname) > 13) {
    printString("cp: invalid destination filename\n");
    return;
  }

  for (i = 0; i < FS_MAX_NODE; i++) {
    if (node_fs_buf.nodes[i].parent_index == dst_parent_index &&
        strcmp(node_fs_buf.nodes[i].node_name, dstname) == 1) {
      printString("cp: destination file already exists\n");
      return;
    }
  }

  dst_meta.parent_index = dst_parent_index;
  strcpy(dst_meta.node_name, dstname);
  dst_meta.filesize = src_meta.filesize;

  for (i = 0; i < src_meta.filesize; i++) {
    dst_meta.buffer[i] = src_meta.buffer[i];
  }

  fsWrite(&dst_meta, &status);

  if (status != FS_SUCCESS) {
    printString("cp: failed to write file\n");
    return;
  }
}

// // TODO: 10. Implement cat function
void cat(byte cwd, char *filename) {
  struct node_fs node_fs_buf;
  struct file_metadata file_meta;
  enum fs_return status;
  int i, file_index = -1;

  readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  for (i = 0; i < FS_MAX_NODE; i++) {
    if (node_fs_buf.nodes[i].parent_index == cwd && 
        strcmp(node_fs_buf.nodes[i].node_name, filename) == 1) { 
      if (node_fs_buf.nodes[i].data_index != FS_NODE_D_DIR) {
        file_index = i;
        break;
      } else {
        printString("cannot read directory\n");
        return;
      }
    }
  }

  if (file_index == -1) {
    printString("file not found\n");
    return;
  }

  file_meta.parent_index = cwd;
  strcpy(file_meta.node_name, filename);
  fsRead(&file_meta, &status);

  if (status != FS_SUCCESS) {
    printString("cat: failed to read file\n");
    return;
  }

  file_meta.buffer[SECTOR_SIZE * FS_MAX_SECTOR - 1] = '\0';
  printString(file_meta.buffer);
  printString("\n");
}

// // TODO: 11. Implement mkdir function
void mkdir(byte cwd, char* dirname) {
  struct node_fs node_buf;
  int i, empty_idx = -1;

  readSector(&node_buf.nodes[0], FS_NODE_SECTOR_NUMBER);
  readSector(&node_buf.nodes[32], FS_NODE_SECTOR_NUMBER + 1);

  for (i = 0; i < FS_MAX_NODE; i++) {
    if (node_buf.nodes[i].parent_index == cwd &&
        strcmp(node_buf.nodes[i].node_name, dirname)) {
      printString("mkdir: folder already exists\n");
      return;
    }
  }

  for (i = 1; i < FS_MAX_NODE; i++) { 
    if (node_buf.nodes[i].node_name[0] == '\0') {
      empty_idx = i;
      break;
    }
  }

  if (empty_idx == -1) {
    printString("mkdir: no space left to create folder\n");
    return;
  }

  node_buf.nodes[empty_idx].parent_index = cwd;
  node_buf.nodes[empty_idx].data_index = FS_NODE_D_DIR;  
  strcpy(node_buf.nodes[empty_idx].node_name, dirname);

  writeSector(&node_buf.nodes[0], FS_NODE_SECTOR_NUMBER);
  writeSector(&node_buf.nodes[32], FS_NODE_SECTOR_NUMBER + 1);
}

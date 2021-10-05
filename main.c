#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <tree_sitter/api.h>
#include <sys/stat.h>
#include <sys/mman.h>

// Declare the `tree_sitter_json` function, which is
// implemented by the `tree-sitter-json` library.
TSLanguage *tree_sitter_json();

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Too few arguments: %d\n", argc - 1);
    return 1;
  }

  // Create a parser.
  TSParser *parser = ts_parser_new();

  // Set the parser's language (JSON in this case).
  ts_parser_set_language(parser, tree_sitter_json());

  // Build a syntax tree based on source code stored in the provided file.
  int fd = open(argv[1], O_RDONLY);
  char *msg = NULL;

  if (fd < 0) {
    asprintf(&msg, "Couldn't open file %s", argv[1]);
    goto die;
  }

  struct stat filestats;
  int status = fstat(fd, &filestats);
  if (status < 0) {
    asprintf(&msg, "Couldn't stat file %s", argv[1]);
    goto die;
  }

  const char *source_code = mmap(0, filestats.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (source_code == MAP_FAILED) {
    asprintf(&msg, "Couldn't mmap file %s", argv[1]);
    goto die;
  }

  TSTree *tree = ts_parser_parse_string(
    parser,
    NULL,
    source_code,
    strlen(source_code)
  );
  if (tree == NULL) {
    asprintf(&msg, "Couldn't parse file for unknown reasons.");
    goto die;
  }

  // Get the root node of the syntax tree.
  TSNode root_node = ts_tree_root_node(tree);

  // Print the syntax tree as an S-expression.
  char *string = ts_node_string(root_node);
  printf("Syntax tree: %s\n", string);

  // Free all of the heap-allocated memory.
  free(string);
  ts_tree_delete(tree);
  ts_parser_delete(parser);
  return 0;

 die:
  perror(msg);
  free(msg);
  return 1;
}

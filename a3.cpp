#include <stdio.h> 
#include <stdlib.h> 
#include "byutr.h"
#include <iostream>
#include <inttypes.h>
#include <unistd.h>
using namespace std;
#define OOPS 5

int N_FLAG = 0;
int O_FLAG = 0;
/*
 * Example of a program that uses the Brigham Young University
 * trace reader.  You do not need this program, but you will
 * need to do what's in it to make your program work.

 * DO NOT INCLUDE THIS IN YOUR PROGRAM
 * USE IT FOR AN EXAMPLE OF HOW TO READ THE TRACE FILE
 * gcc -std=c11 -o sample_read sample_read.c byu_tracereader.c
 */

int main(int argc, char **argv) {
  int option;
  int samples;
  char argument[256];
  char *o_value;
  int *n_value;
  FILE *fp = fopen("trace.sample.tr", "r");
  cout << argc << "\n";
  int i = 0;
  for(int i ; i<argc; i++){
    printf("%s\n",argv[i]);
  }

  if (fp == NULL) {
    /* couldn't open, bail gracefully */
    printf("uh oh\n");
    exit(OOPS);
  }
  while ((option = getopt(argc, argv, "o:n:")) != -1) {
      switch (option) {
        case 'n':
              try {
                    if ((samples = stoi(optarg, nullptr, 10)) <= 0) {
                        fprintf(stderr,
                                "Option -%c argument must be a number > 0: %s\n",
                                optopt, optarg);
                        return EXIT_FAILURE;
                    }
                }
              catch (const invalid_argument &ia) {
                    cerr << "Option -n argument must be a number." << endl;
                    return EXIT_FAILURE;
                }
              n_value = &samples; // All checks for n value passed
              N_FLAG++;
              n_value = &samples;
              printf("N COUNT : %d\n", N_FLAG);
              break;
        case 'o':
            //TODO CHECKS FOR THE FOLLOWING 
            //bitmasks, logical2physical, page2frame, offset, summary
              O_FLAG++;
              printf("O COUNT : %d\n", O_FLAG);
              o_value = optarg;
              printf("argument : %s\n", o_value);
              break;
        default : 
          break;
                
     }
    }
    printf("OPTION INDEX : %i\nARGUMENT COUNT: %i\n", optind, argc);
    if (optind >= argc) {
        cerr << "Expected argument after options" << endl;
        return EXIT_FAILURE;
    } else if (argc - optind < 1) {
        cerr << "Expected at least one page level" << endl;
        return EXIT_FAILURE;
    }

    // char *traceFilePath = argv[optind++];
    // int remainingArgs = argc - optind;
  printf("%p\n", fp);
  fclose(fp);
  cout << "end of Program" << "\n";
  
return 0;
}
#include <stdio.h> 
#include <stdlib.h> 
#include "byutr.h"
#include <iostream>
#include <inttypes.h>
#include <unistd.h>
#include <vector>

#include "page_table.h"
#include "output_mode_helpers.h"

using namespace std;
#define OOPS 5

int NO_FLAG = 0;
int N_FLAG = 0;
int O_FLAG = 0;
OutputOptionsType argumentations;

int BIT_FLAG = 0;
int SUMMARY_FLAG = 0;
int L2P_FLAG = 0;
int OFFSET_FLAG = 0;
int P2F_FLAG = 0;
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
 
  // cout << argc << "\n";
  int i = 0;
  // for(int i ; i<argc; i++){
  //   printf("%s\n", argv[i]);
  // }

  
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
              // printf("N COUNT : %d\n", N_FLAG);
              // printf("FLAG NUMBER: %i\n", *n_value);
              break;
        case 'o':
            //TODO CHECKS FOR THE FOLLOWING 
            //bitmasks, logical2physical, page2frame, offset, summary
              O_FLAG++;
              // printf("O COUNT : %d\n", O_FLAG);
              o_value = optarg;
              if(strcmp(o_value, "bitmasks") == 0){
                argumentations.bitmasks = true;
                BIT_FLAG++;
              }
              if(strcmp(o_value, "summary") == 0) {
                argumentations.summary = true;
                SUMMARY_FLAG++;
              }
               if(strcmp(o_value, "offset") == 0){
                 argumentations.offset = true;
                 OFFSET_FLAG++;
               }
               if(strcmp(o_value, "page2frame") == 0){
                 argumentations.page2frame = true;
                 P2F_FLAG++;
               }
               if(strcmp(o_value, "logical2physical") == 0){
                 argumentations.logical2physical = true;
                L2P_FLAG++;
              }
              // printf("argument : %s\n", o_value);
              break;
        default : 
          break;
                
     }
    }
    if(!argumentations.bitmasks 
    && !argumentations.logical2physical 
    && !argumentations.offset 
    && !argumentations.page2frame
    && !argumentations.summary){
      if(O_FLAG > 0){
      printf("The Argument: \'%s\' is not a valid argument after -o\n", o_value); 
      return EXIT_FAILURE;}
      else{
        NO_FLAG++;
      }
    }

    char *fileindex = argv[optind];
    optind++; 
    FILE *fp;
    if((fp = fopen(fileindex, "r")) == nullptr) {
      printf("cannot open %s, double check the file path given", fileindex);
      return EXIT_FAILURE;
      }
    // printf("FILE PATH :%s\n", fileindex);
    // printf("OPTION INDEX : %i\nARGUMENT COUNT: %i\n", optind, argc);
    
    if(optind == 1 )
    {

    }else{
    if (optind >= argc) {
        cerr << "Expected argument after options" << endl;
        return EXIT_FAILURE;
    } else if (argc - optind < 1) {
        cerr << "Expected at least one page level" << endl;
        return EXIT_FAILURE;
    }}
  
  int difference = argc - optind;
 

  // printf("DIFFERENCE :%i\n", difference);
  int pageBitTotal = 0;
  //vector<int> levels;
  vector<int> levels;
  p2AddrTr trace; 
  for (int i = 0; i < difference; i++) {
        int levelBits;
        try {
            if ((levelBits = stoi(argv[optind], nullptr, 10)) <= 0 ) {
                fprintf(stderr, "level bit must be a number > 0: %s\n",
                        argv[optind]);
                return EXIT_FAILURE;
            }
            // printf("optind check : %i\n", optind);
        }
        catch (const invalid_argument &ia) {
            if (difference == 0) {
                fprintf(stderr, "level bit must be a number: %s\n",
                argv[optind]);
                return EXIT_FAILURE;
            }
            break;
        }
        levels.push_back(levelBits);
        pageBitTotal += levelBits;
        //levelCount++;
        optind++;
    }
  if(pageBitTotal >31){
    printf("TOTAL BIT COUNT EXCEEDS 31\n");
    return EXIT_FAILURE;
  }
  PageTable pagetable(difference, levels);
  // printf("DRIVER PAGETABLE: %x\nLEVEL COUNT: %i\n", pagetable.bitmaskAry[1], pagetable.levelCountTable);
  // printf("ENTRY COUNT: %i\n", pagetable.entrycount[1]);
  // printf("SHIFT COUNT: %i\n", pagetable.shiftAry[1]);
  // printf("TOTAL BIT NUM : %i\n", pageBitTotal);
  // printf("OFFSET: %i\n", 32 - pageBitTotal);
  uint32_t frame = 0;
  int run = 0;
  while (!feof(fp)){
    if(NextAddress(fp, &trace)) {
      uint32_t logicalAddr = trace.addr;
      pagetable.pageInsert(logicalAddr);
      if(argumentations.offset){
        uint32_t off = logicalAddr<<pageBitTotal;
        report_logical2offset(logicalAddr, off>>pageBitTotal);
      } else if (argumentations.page2frame) {
        pagetable.pageToFrame();
      }else if(argumentations.logical2physical) {
          uint32_t physical;
          pagetable.logicalToPhysical(logicalAddr, physical, (32-pageBitTotal));
      }
      //printf("address: %x\n", logicalAddr);
    }
    run++;
    if(*n_value == run){break;}
    
  }
  if(argumentations.bitmasks){
    report_bitmasks(difference, pagetable.bitmaskAry);
  } else if (argumentations.summary || NO_FLAG > 0){
    report_summary(pow(2, 32 - pageBitTotal), pagetable.hits, 
		    pagetable.totalADDRS, (pagetable.totalADDRS-pagetable.hits), pagetable.totalMemory);
  }
//printf("FINAL COUNT: %i\t%i\t%i\n", pagetable.misses, pagetable.hits, pagetable.totalMemory);
fclose(fp);
  // cout << "end of Program" << "\n";
EXIT_SUCCESS;
return 0;
}
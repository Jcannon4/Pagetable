#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include "byutr.h"
#include <iostream>
#include <inttypes.h>
#include <unistd.h>
#include <vector>
#include <math.h>       

#include "page_table.h"
#include "output_mode_helpers.h"

using namespace std;
#define OOPS 5

int NO_FLAG = 0;
int N_FLAG = 0;
int O_FLAG = 0;
OutputOptionsType argumentations;

//THIS ACCOMPLISHES THE SAME THING AS
//THE OUTPUT OPTIONS TYPE
int BIT_FLAG = 0;
int SUMMARY_FLAG = 0;
int L2P_FLAG = 0;
int OFFSET_FLAG = 0;
int P2F_FLAG = 0;


int main(int argc, char **argv) {
  int option;
  int samples;
  char argument[256];
  char *o_value;
  int *n_value;

  int i = 0;
  
  while ((option = getopt(argc, argv, "o:n:")) != -1) {
      switch (option) {
        case 'n':
              //ENSURE WE HAVE A NUMBER FOLLOWING OUR N FLAG
                if(isdigit(optarg[0]))
                {
                  //MAKE SURE STRING IS A POSITIVE INTEGER ABOVE 0
                    if ((samples = stoi(optarg, nullptr, 10)) <= 0)
                    {
                      fprintf(stderr, "Option -%c argument must be a number > 0: %s\n",
                                optopt, optarg);
                        return EXIT_FAILURE;
                    }
                }
                else 
                {
                  //NON DIGIT DETECTED, EXIT PROGRAM
                  printf("Argument \'%s\' entered is not a number\n", optarg); 
                  exit(0);
                }
                //STORE N VALUE, SIGNAL THAT N FLAG WAS FOUND
              n_value = &samples; 
              N_FLAG++;
              n_value = &samples;
            
              break;
        case 'o':
              O_FLAG++;
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
      //CHECKS THAT A ARGUMENT WAS SUPPLIED AFTER -O FLAG
      if(O_FLAG > 0){
      printf("The Argument: \'%s\' is not a valid argument after -o\n", o_value); 
      return EXIT_FAILURE;}
      else{
        //NO -O FLAG FOUND SO BY DEFAULT WE WILL RUN SUMMARY
        NO_FLAG++;
      }
    }
    //FILE SHOULD BE SUPPLIED AFTER OPTARG
    char *fileindex = argv[optind];
    optind++; //MOVE OPTIND TO GRAB FILE
    FILE *fp;
    if((fp = fopen(fileindex, "r")) == nullptr) {
      printf("cannot open %s, double check the file path given", fileindex);
      return EXIT_FAILURE;
      }
    
    if(optind == 1 )
    {

    }else{
    if (optind >= argc) {
        cerr << "No argument given after options" << endl;
        return EXIT_FAILURE;
    } else if (argc - optind < 1) {
        cerr << "There needs to be at least one page level" << endl;
        return EXIT_FAILURE;
    }}
  //LEVEL CALCULATIONS
  int difference = argc - optind;
  int pageBitTotal = 0;
  vector<int> levels;
  p2AddrTr trace; 
  for (int i = 0; i < difference; i++) {
        int levelBits;
            if ((levelBits = stoi(argv[optind], nullptr, 10)) <= 0 ) {
                fprintf(stderr, "Level bit must be nonzero positive integer: %s\n", argv[optind]);
                return EXIT_FAILURE;
            }
            if (difference == 0) {
                fprintf(stderr, "level bit should be a number: %s\n", argv[optind]);
                return EXIT_FAILURE;
            }
        levels.push_back(levelBits);
        pageBitTotal += levelBits;
        optind++;
    }
  if(pageBitTotal >31){
    printf("TOTAL BIT COUNT EXCEEDS 31\n");
    return EXIT_FAILURE;
  }
  //CREATE OUR PAGETABLE WITH LEVEL FUNCTIONS
  PageTable pagetable(difference, levels);
  
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
        pagetable.pageToFrame(logicalAddr);

      }else if(argumentations.logical2physical) {
         
          pagetable.logicalToPhysical(logicalAddr, (32-pageBitTotal));
        }
      
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
fclose(fp);
EXIT_SUCCESS;
return 0;
}
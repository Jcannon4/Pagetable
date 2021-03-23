#include "stdlib.h"
#include "page_table.h"
#include "math.h"
#include "page_table.h"
#include "output_mode_helpers.h"
#include <vector>

using namespace std;

PageTable::PageTable(int levelCount, vector<int> levelAry){
    
    int ADDRS = 31;
    uint32_t bitmasks[levelCount];
    for (int i = 0; i < levelCount ; i++){
        uint32_t tmp = 0x0;
        for (int j = 0; j < levelAry[i]; j++){
            tmp += pow(2, ADDRS);
            
            ADDRS--;
        }
        bitmasks[i] = tmp;
    }
    report_bitmasks(levelCount, bitmasks);
}
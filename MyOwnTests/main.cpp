//
//  main.cpp
//  binpack
//
//  Created by Holger Schmidt on 29.11.19.
//  Copyright © 2019 Holger Schmidt. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include "MaxRectsBinPack.h"

using namespace rbp;

int main(int argc, const char * argv[]) {
    
//    if (argc < 5 || argc % 2 != 1)
//    {
//        printf("Usage: MaxRectsBinPackTest binWidth binHeight w_0 h_0 w_1 h_1 w_2 h_2 ... w_n h_n\n");
//        printf("       where binWidth and binHeight define the size of the bin.\n");
//        printf("       w_i is the width of the i'th rectangle to pack, and h_i the height.\n");
//        printf("Example: MaxRectsBinPackTest 256 256 30 20 50 20 10 80 90 20\n");
//        return 0;
//    }
    
    auto heuristics = {MaxRectsBinPack::RectBestShortSideFit,
        MaxRectsBinPack::RectBottomLeftRule,
        MaxRectsBinPack::RectContactPointRule,
        MaxRectsBinPack::RectBestLongSideFit,
        MaxRectsBinPack::RectBestAreaFit
    };
    
    const std::vector<RectSize> pages{RectSize{6, 97}};
    
    const std::vector<std::vector<RectSize>> containers{{
        RectSize{5, 30},
        RectSize{4, 41},
        RectSize{2, 22},
        RectSize{1, 30},
        RectSize{1, 41},
        RectSize{1, 31},
        RectSize{1, 14}}};
    
    const std::vector<std::vector<Rect>> blockers = {{Rect{0, 73, 3, 23}, Rect{5, 75, 1, 21}}};
    
    for (size_t p = 0; p < pages.size(); ++p) {
        auto &page = pages[p];
        
        for (auto &heuristic : heuristics)
        {
            switch (heuristic) {
                case MaxRectsBinPack::RectBestShortSideFit: printf("heuristic RectBestShortSideFit\n"); break;
                case MaxRectsBinPack::RectBottomLeftRule: printf("heuristic RectBottomLeftRule\n"); break;
                case MaxRectsBinPack::RectContactPointRule: printf("heuristic RectContactPointRule\n"); break;
                case MaxRectsBinPack::RectBestLongSideFit: printf("heuristic RectBestLongSideFit\n"); break;
                case MaxRectsBinPack::RectBestAreaFit: printf("heuristic RectBestAreaFit\n"); break;
            }

            // Create a bin to pack to, use the bin size from command line.
            MaxRectsBinPack bin;
            int binWidth = page.width;
            int binHeight = page.height;
            printf("Initializing bin to size %dx%d.\n", binWidth, binHeight);
            bin.Init(binWidth, binHeight, false);
            
            for (auto &blocker : blockers[p]) {
                bin.PlaceRect(blocker);
                printf("blocker (x,y)=(%d,%d), (w,h)=(%d,%d)\n", blocker.x, blocker.y, blocker.width, blocker.height);
            }
            
            std::vector<Rect> placedContainers;
            std::vector<RectSize> containersToPlace(containers[p].begin(), containers[p].end());
            bin.Insert(containersToPlace, placedContainers, heuristic);
            
            for (auto &packedRect : placedContainers) {
                // Test success or failure.
                if (packedRect.height > 0)
                    printf("Packed to (x,y)=(%d,%d), (w,h)=(%d,%d). Free space left: %.2f%%\n", packedRect.x, packedRect.y, packedRect.width, packedRect.height, 100.f - bin.Occupancy()*100.f);
                else
                    printf("Failed! Could not find a proper position to pack this rectangle into. Skipping this one.\n");
            }
            
            printf("Done. Rectangles packed.\n\n\n");
            
            /*
            for(int i = 1; i < page.size(); ++i)
            {
                // Read next rectangle to pack.
                int rectWidth = page[i].width;
                int rectHeight = page[i].height;
                printf("Packing rectangle of size %dx%d: ", rectWidth, rectHeight);

                // Perform the packing.
                Rect packedRect = bin.Insert(rectWidth, rectHeight, heuristic);

                // Test success or failure.
                if (packedRect.height > 0)
                    printf("Packed to (x,y)=(%d,%d), (w,h)=(%d,%d). Free space left: %.2f%%\n", packedRect.x, packedRect.y, packedRect.width, packedRect.height, 100.f - bin.Occupancy()*100.f);
                else
                    printf("Failed! Could not find a proper position to pack this rectangle into. Skipping this one.\n");
            }
            printf("Done. All rectangles packed.\n\n\n");
             */
        }
    }
}

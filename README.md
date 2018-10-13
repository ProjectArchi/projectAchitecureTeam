# projectAchitecureTeam
projectAchitecureTeam
Notes for functions.h:
Content: 1. wbBuffer for each exe unit which should be initialized in CPU for saving result from exe
         2. Unit: Implemented by circular queue, init in CPU and save instructions in exe stage
         3. Commit: static method for commit stage
         4. WB: static method for write back stage
         5. Execute stage: static method for exe stage
         6. Commit, WB, EXE should be placed in clockwise
About BTB: EXE: 1. Add entry while predict not taken yet actually taken
                2. Remove entry while predict taken yet actually not taken
           Commit: 1. move PC if mispredicted
                   2. clean up ROB and register status if mispredicted

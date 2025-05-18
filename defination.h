//宏定义							    
#define    SPEED                   20  //地鼠移动间隔时间

#define    WINDOW_WIDTH            640 //窗口宽
#define    WINDOW_HEITH            478 //窗口宽

#define    RULE_WIDTH              800 //规则窗口宽
#define    RULE_HEIGH              601 //规则窗口高

#define    TOP_HEIGH               192 //背景图top高
#define    MID_UP_HEIGH            120 //背景图mid_up高
#define    MID_DOWN_HEIGH          167 //背景图mid_down高
#define    BOTTOM_HEIGH            116 //背景图bottom高

#define    MID_UP_BEGIN_Y          164 //背景图mid_up起始y坐标
#define    MID_DOWN_BEGIN_Y        247 //背景图mid_down起始y坐标
#define    BOTTOM_BEGIN_Y          364 //背景图bottom起始y坐标

#define    LITTLE_MOUSE_WIDTH      90  //小地鼠宽
#define    LITTLE_MOUSE_HEIGH      74  //小地鼠高
#define    MID_MOUSE_WIDTH         102 //中地鼠宽
#define    MID_MOUSE_HEIGH         86  //中地鼠高
#define    BIG_MOUSE_WIDTH         122 //大地鼠宽
#define    BIG_MOUSE_HEIGH         106 //大地鼠高

#define    LITTLE_MOUSE_BEGIN_X    135 //小地鼠起始位置
#define    LITTLE_MOUSE_BEGIN_Y    191 
#define    LITTLE_MOUSE_SPACE      137 //小地鼠间距

#define    MID_MOUSE_BEGIN_X       106 //中地鼠起始位置
#define    MID_MOUSE_BEGIN_Y       283 
#define    MID_MOUSE_SPACE         156 //中地鼠间距

#define    BIG_MOUSE_BEGIN_X       65  //大地鼠起始位置
#define    BIG_MOUSE_BEGIN_Y       410 
#define    BIG_MOUSE_SPACE         185 //大地鼠间距

#define    UP                      1   //地鼠运动方向向上
#define    DOWN                    0   //地鼠运动方向向下

#define    HAMMER_BEGIN_X          320 //锤子初始位置
#define    HAMMER_BEGIN_Y          220 

#define    SCORE_WORDS_X           5   //"得分"文字描述与数字的位置
#define    SCORE_NUMBER_X          90  
#define    SCORE_Y                 30  

#define    TARGET_WORDS_X          5   //"目标"文字描述与数字的位置
#define    TARGET_NUMBER_X         90  
#define    TARGET_Y                10  

#define    MISSED_WORDS_X          470 //"错过的"文字描述与数字的位置
#define    MISSED_NUMBER_X         610 
#define    MISSED_Y                10  

#define    MAX_MISSED_WORDS_X      490 //"生命值"文字描述与数字的位置
#define    MAX_MISSED_NUMBER_X     610 
#define    MAX_MISSED_Y            30  

#define    EFFECTIVE_ZONE_X        36  
#define    EFFECTIVE_ZONE_Y        101 
#define    EFFECTIVE_ZONE_HEIGHT   33  
#define    EFFECTIVE_ZONE_WIDTH    64  

#define    FRAME                   20  //锤子砸下状态转换持起状态的间隔帧

#define    time_interal            100 //从地鼠被打倒后,到再次刷新地鼠的时间间隔

#define    SIMPLE_TARGET           20  //简单模式下需要锤的个数
#define    MIDDLE_TARGET           30  //中等模式下需要锤的个数
#define    HARD_TARGET             45  //困难模式下需要锤的个数
#define    HEEL_TARGET             70  //地狱模式下需要锤的个数

#define    SIMPLE_MAXMISS          20  //简单模式下最大可以错过地鼠的个数
#define    MIDDLE_MAXMISS          15  //中等模式下最大可以错过地鼠的个数
#define    HARD_MAXMISS            10  //困难模式下最大可以错过地鼠的个数
#define    HEEL_MAXMISS            5   //地狱模式下最大可以错过地鼠的个数

#define    SIMPLE_SPEED            3   //简单模式下地鼠的速度
#define    MIDDLE_SPEED            5   //中等模式下地鼠的速度
#define    HARD_SPEED              9   //困难模式下地鼠的速度
#define    HEEL_SPEED              11  //地狱模式下地鼠的速度
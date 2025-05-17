/*开发日志
 1.创建项目
 2.导入素材
 3.c++开发
 4.设计c++项目模块
   设计类： Block（方块）， Tetris（游戏）
 5.设计各个模块接口
 6.启动游戏
   
 7.
*/


#include"Tetris.h"

int main(void) {
	Tetris game(20, 10, 263, 133, 36);
	game.play();
	return 0;
}
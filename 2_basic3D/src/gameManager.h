#include <iostream>

using namespace std;

class GameManager {

    private :
        int mapSize = 11;
        int map[11][11] = 
        {
            {1,1,1,1,1,2,1,1,1,1,1},
            {1,1,1,0,0,0,0,0,0,0,1},
            {1,1,1,0,0,0,1,0,0,0,1},
            {1,0,0,1,0,0,0,1,1,0,1},
            {1,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,1,0,0,0,1,1},
            {1,0,0,0,0,0,0,0,0,1,1},
            {1,1,1,1,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,1,1,1,1},
            {1,0,0,0,0,4,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,1},
        };

        //starting position
        int px = 5;
        int py = 9;

    public :
        bool gameEnd = false;

        int getPx() {
            return px;
        }
        int getPy() {
            return py;
        }

        void printMap() {
            for(int i = 0; i < mapSize; i++) {
                for(int j = 0; j < mapSize; j++) {
                    cout << map[i][j] << "|";
                }
                cout << "\n";
            }
        }

        void up() {
            for(int i = py-1; i >= 0; i--) {
                // cout << i << ", " << px << " found " << map[i][px] << "\n";
                if(map[i][px] == 1) {
                    // cout << "hit!\n";
                    // map[py][px] = 0;
                    // map[i+1][px] = 4;
                    py = i+1;
                    break;
                }
                else if(map[i][px] == 2) {
                    gameEnd = true;
                    // map[py][px] = 0;
                    // map[i][px] = 4;
                    py = i;
                    break;
                }
            }
        }
        void left() {
            for(int i = px-1; i >= 0; i--) {
                // cout << py << ", " << i << " found " << map[py][i] << "\n";
                if(map[py][i] == 1) {
                    // cout << "hit!\n";
                    // map[py][px] = 0;
                    // map[py][i+1] = 4;
                    px = i+1;
                    break;
                }
                else if(map[py][i] == 2) {
                    gameEnd = true;
                    // map[py][px] = 0;
                    // map[py][i] = 4;
                    px = i;
                    break;
                }
            }
        }
        void down() {
            for(int i = py+1; i < mapSize; i++) {
                // cout << i << ", " << px << " found " << map[i][px] << "\n";
                if(map[i][px] == 1) {
                    // cout << "hit!\n";
                    // map[py][px] = 0;
                    // map[i-1][px] = 4;
                    py = i-1;
                    break;
                }
                else if(map[i][px] == 2) {
                    gameEnd = true;
                    // map[py][px] = 0;
                    // map[i][px] = 4;
                    py = i;
                    break;
                }
            }
        }
        void right() {
            for(int i = px+1; i < mapSize; i++) {
                // cout << py << ", " << i << " found " << map[py][i] << "\n";
                if(map[py][i] == 1) {
                    // cout << "hit!\n";
                    // map[py][px] = 0;
                    // map[py][i-1] = 4;
                    px = i-1;
                    break;
                }
                else if(map[i][py] == 2) {
                    gameEnd = true;
                    // map[py][px] = 0;
                    // map[i][py] = 4;
                    px = i;
                    break;
                }
            }
        }

};

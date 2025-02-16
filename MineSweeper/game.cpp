//necessary libraries
#include <bits/stdc++.h>
#include <windows.h> //getch()
#include <conio.h>

using namespace std; //for cout, mainly.

#define fst ios_base::sync_with_stdio(0); cin.tie(NULL);

//---unicode characters for the grid---

const string TopLeftCorner = "\u2554";
const string TopRightCorner = "\u2557";
const string BottomLeftCorner = "\u255A";
const string BottomRightCorner = "\u255D";

const string HorizontalBar = "\u2550";
const string VerticallBar = "\u2551";

const string ThreeWayUp = "\u2569";
const string ThreeWayDown = "\u2566";
const string ThreeWayLeft = "\u2563";
const string ThreeWayRight = "\u2560";

const string FourWay = "\u256C";

//----game data-----
struct{
    int pos[2] = {1, 2}, //cursor position
    TableSize[2],
    TableNumbers[40][20]; //-1 for bomb, 1-8 for numbers
    
    clock_t StartTime, FullTime;

    int BombCount, FlagCount, HiddenCount, score;

    bool started, //to distinguish the first action of the player 
    bomb[40][20], //bomb positions in the grid
    flag[40][20], //flag positions in the grid
    show[40][20]; //whether to show the bombs/numbers or not

    string PlayerName,
    Options[4] = {
        "New Game",
        "How To Play",
        "Leaderboard",
        "Exit"
    },
    Difficulties[4] = {
        "Easy",
        "Medium",
        "Hard",
        "Back"
    };

} game;

//----Function Initialization-----

void gotoxy(short, short, string s = ""); //cursor positioning

void Color(int, int); //set the output color(foreground, background)
void Logo(); //the logo shown in menu pages
void PrintChar(string, int); //prints string s for n times

void ConstructTable(int, int); //builds and outputs the table given the input rows and columns based on difficulity
void SetTableNumbers(int, int, int, int, int); //randomly places bombs and respectively numbers, given the width, height and the position of the player's first clearance

void StartGame(); //starts the game with previous initializations done
void unroll(int x, int y); //reveals the neccessary cells based on the cell's (x, y) content 

void Menu(); //starts the menu procedure
void Instruction(); //instructions page
void ButtonSwitch(int, int); //for switching between menu Options

void SetDifficulty(); //The menu page dedicated for choosing the difficulity level
void SetValue(int, int, int); //initializes the game with the given row, column and bomb count
void SetFlag(int, int); //for flag/unflagging a cell

void EndGame(bool, string, string); //end of game procedure, based on win or lost 

void Create(); //creates the scoreboard file if neccessary
void ShowScore(); //scoreboard page
void UpdateScore(string, int, int); //updates the scoreboard with given data

int score(); //calculates player score

void Solve();
int Check(int, int);
//--------------------------

int main(){
    Menu(); //game starts with calling the main menu
    return 0;
}

void gotoxy(short x, short y, string s){
    //prints string s with null default value in pos(x, y) of the std output.
    HANDLE h_std = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {x, y};
    SetConsoleCursorPosition(h_std, pos); 
    cout<<s;
}

void Color(int f, int b){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (b << 4) | f);
}

void logo(){
    system("cls");

    cout<<endl;
    Color(1, 0);
    cout<<"         __  __ _                           "<<endl;
    Color(2, 0);
    cout<<"        |  \\/  (_)_ __   ___                "<<endl;
    Color(3, 0);
    cout<<"        | |\\/| | |  _ \\ / _ \\               "<<endl;
    Color(4, 0);
    cout<<"  ____  | |  | | | | | |  __/               "<<endl;
    Color(5, 0);
    cout<<" / ___|_|_|  |_|_|_| |_|\\___|   ___ _ __    "<<endl;
    Color(6, 0);
    cout<<" \\___ \\ \\ /\\ / / _ \\/ _ \\ '_ \\ / _ \\ '__|   "<<endl;
    Color(7, 0);
    cout<<"  ___) \\ V  V /  __/  __/ |_) |  __/ |      "<<endl;
    Color(8, 0);
    cout<<" |____/ \\_/\\_/ \\___|\\___| .__/ \\___|_|      "<<endl;
    Color(4, 0);
    cout<<"                        |_|                 "<<endl;
    cout<<endl;
}

void PrintChar(string s, int n){
    while(n--)
        cout<<s;
}

void ConstructTable(int column, int row){
    system("cls");

    //Top Row
    cout<<TopLeftCorner;
    
    for(int i = 0; i < column - 1; i++){
        PrintChar(HorizontalBar, 3);
        cout<<ThreeWayDown;
    }

    PrintChar(HorizontalBar, 3);
    cout<<TopRightCorner<<endl;
    
    //Middle Rows
    for(int i = 0; i < row - 1; i++){
        for(int j = 0; j < column; j++){
            cout<<VerticallBar;
            PrintChar(" ", 3);
        }
       
        cout<<VerticallBar<<endl;
        cout<<ThreeWayRight;
        
        for(int j = 0; j < column - 1; j++){
            PrintChar(HorizontalBar, 3);
            cout<<FourWay;
        }
        
        PrintChar(HorizontalBar, 3);
        cout<<ThreeWayLeft<<endl;
    }
    
    for(int i = 0; i < column + 1; i++){
        cout<<VerticallBar;
        PrintChar(" ", 3);
    }

    //Bottom Row
    cout<<endl<<BottomLeftCorner;
    
    for(int i = 0; i < column - 1; i++){
        PrintChar(HorizontalBar, 3);
        cout<<ThreeWayUp;
    }
    
    PrintChar(HorizontalBar, 3);
    cout<<BottomRightCorner;

    cout<<endl<<endl<<"Flags Remaining: "<<game.FlagCount<<endl<<endl;
    
    gotoxy(game.pos[0], game.pos[1]);
}

void SetTableNumbers(int height, int width, int bombcount, int x, int y){
    game.TableNumbers[x][y] = 0; //to ensure the first chosen cell is empty
    
    int RandX, RandY; //random postion for a bomb
    
    srand(time(0)); //random seed for bomb positions
    
    //to place bombs in a random (and new) postion
    while(bombcount){
        RandX = rand() % height;
        RandY = rand() % width;
        if((abs(RandX - x) > 1 || abs(RandY - y) > 1) && !game.bomb[RandX][RandY])
            game.bomb[RandX][RandY] = true, game.TableNumbers[RandX][RandY] = -1, bombcount--;
    }

    //to set table numbers based on bomb position
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(game.TableNumbers[i][j] > -1){
                if(game.TableNumbers[i - 1][j] == -1 && i > 0)
                    game.TableNumbers[i][j]++;

                if(game.TableNumbers[i + 1][j] == -1 && i < height - 1)
                    game.TableNumbers[i][j]++;

                if(game.TableNumbers[i][j - 1] == -1 && j > 0)
                    game.TableNumbers[i][j]++;

                if(game.TableNumbers[i][j + 1] == -1 && j < width - 1)
                    game.TableNumbers[i][j]++;

                if(game.TableNumbers[i - 1][j - 1] == -1 && i > 0 && j > 0)
                    game.TableNumbers[i][j]++;

                if(game.TableNumbers[i - 1][j + 1] == -1 && i > 0 && j < width - 1)
                    game.TableNumbers[i][j]++;

                if(game.TableNumbers[i + 1][j - 1] == -1 && i < height - 1 && j > 0)
                    game.TableNumbers[i][j]++;

                if(game.TableNumbers[i + 1][j + 1] == -1 && i < height - 1 && j < width - 1)
                    game.TableNumbers[i][j]++;
            }
        }
    }

    unroll(x, y);
}

void StartGame(){
    gotoxy(2, 1);

    game.started = 0;
    char move;

    //receive player input (WASD - F for flag - C for clear) and initialize the neccessary procedures 
    while(true){
        move = getch();
        switch(move){
        case 'w':
            if(game.pos[1] != 1){
                game.pos[1] += -2;
                gotoxy(game.pos[0], game.pos[1]);
            }
            break;
        case 's':
            if(game.pos[1] != 2 * game.TableSize[1] - 1){
                game.pos[1] += 2;
                gotoxy(game.pos[0], game.pos[1]);
            }
            break;
        case 'a':
            if(game.pos[0] != 2){
                game.pos[0] += -4;
                gotoxy(game.pos[0], game.pos[1]);
            }
            break;
        case 'd':
            if(game.pos[0] != 4 * game.TableSize[0] - 2){
                game.pos[0] += 4;
                gotoxy(game.pos[0], game.pos[1]);
            }
            break;
        case 'f':
            SetFlag(game.pos[0], game.pos[1]);
            break;
        case 'v':
            if(game.started)
                Solve();
            break;
        case 'p':
            exit(0);
            break;
        case 'c':
            if(!game.started){ //is it the first time player has pressed this thing?
                SetTableNumbers(game.TableSize[0], game.TableSize[1], game.BombCount, (game.pos[0] - 2) / 4, (game.pos[1] - 1) / 2);
                gotoxy(game.pos[0], game.pos[1]);

                game.started = 1;
                game.StartTime = clock();
            }
            else if(!game.flag[(game.pos[0] - 2) / 4][(game.pos[1] - 1) / 2]){
                unroll((game.pos[0] - 2) / 4, (game.pos[1] - 1) / 2);
                gotoxy(game.pos[0], game.pos[1]);
            }
            break;
        default:
            break;
        }
    }
}

void unroll(int x, int y){
    if(game.show[x][y]) //already revealed, go back
        return;

    if(!game.TableNumbers[x][y]){ //it's empty, do a recursive calling for its adjacent cells 
        Color(7, 0);
        gotoxy(4 * x + 2, 2 * y + 1, "-");
        game.show[x][y] = true, game.HiddenCount--;

        if(game.HiddenCount == game.BombCount) //endgame if all clear cells have been cleared
            EndGame(1, "F", "Won");

        //recursive calls
        if(x >= 1)
            unroll(x - 1, y);
        if(x < game.TableSize[0] - 1)
            unroll(x + 1, y);
        if(y >= 1)
            unroll(x, y - 1);
        if(y < game.TableSize[1] - 1)
            unroll(x, y + 1);
        if(x >= 1 && y >= 1)
            unroll(x - 1, y - 1);
        if(x >= 1 && y < game.TableSize[1] - 1)
            unroll(x - 1, y + 1);
        if(x < game.TableSize[0] - 1 && y >= 1)
            unroll(x + 1, y - 1);
        if(x < game.TableSize[0] - 1 && y < game.TableSize[1] - 1)
            unroll(x + 1, y + 1);

    }
    else if(game.TableNumbers[x][y] > 0){ //it's a number, 
        Color((game.TableNumbers[x][y] != 4 ? game.TableNumbers[x][y] : 11), 0);
        gotoxy(4 * x + 2, 2 * y + 1, to_string(game.TableNumbers[x][y]));
        game.show[x][y] = true, game.HiddenCount--;
        Color(7, 0);
        
        if(game.HiddenCount == game.BombCount) //endgame if all clear cells have been cleared
            EndGame(1, "F", "Won");
        
        return;
    }
    else if(game.TableNumbers[x][y] == -1){ //bomb clicked, endgame lost procedure
        EndGame(0, "*", "Lost");
    }
}

void Menu(){
    int index = 0;
    ButtonSwitch(index, 0);

    //navigate/choose menu buttons based on player input (WS, ENTER)
    while(true){
        char op = getch();
        switch(op){
            case 'w':
                index = max(index - 1, 0);
                ButtonSwitch(index, 0);
                break;
            case 's':
                index = min(index + 1, 3);
                ButtonSwitch(index, 0);
                break;
            case '\r':
                switch(index){
                    case 0: SetDifficulty(); //difficulity page
                    case 1: Instruction();  //instruction page
                    case 2: ShowScore();   //scoreboard page
                    case 3: exit(0);      //exit game
                }
        }

    }

}

void Instruction(){
    logo();
    Color(7, 0);

    cout<<"    PLAY IN FULLSCREEN MODE PLEASEEEE\n\n";
    cout<<"         Navigate with WASD keys,\n";
    cout<<"       Mark a cell as clear with c,\n";
    cout<<"        Mark a cell as flag with f\n\n\n";
    cout<<"Press any key to continue...";

    char c = getch();
    Menu();
}

void ButtonSwitch(int index, int list_index){
    logo();
    Color(7, 0);

    //Shows arrows around the "index" index of the "list_index" list. 
    for(int i = 0; i < 4; i++){
        cout<<"          ";

        if(i == 3 && list_index == 1)
            cout<<"\n          ";

        cout<<(i == index ? "-> " : "   ");
        cout<<(list_index? game.Difficulties[i] : game.Options[i]);
        cout<<(i == index ? " <-" : "")<<endl;
    }
}

void SetDifficulty(){
    int index = 0;
    ButtonSwitch(index, 1);

    //set game values based on chosen difficulity
    while(true){
        char op = getch();
        switch(op){
            case 'w':
                index = max(index - 1, 0);
                ButtonSwitch(index, 1);
                break;
            case 's':
                index = min(index + 1, 3);
                ButtonSwitch(index, 1);
                break;
            case '\r':
                switch(index){
                    case 0: SetValue(9, 9, 10);
                    case 1: SetValue(16, 16, 40);
                    case 2: SetValue(16, 30, 99);
                    case 3: Menu();
                }
        }

    }
}

void SetValue(int row, int col, int bomb){
    //initialize default game values based on chosen difficulity at the start of the game 
    game.pos[0] = 2, game.pos[1] = 1;

    game.TableSize[0] = col, game.TableSize[1] = row;
    game.BombCount = bomb;
    game.FlagCount = bomb;
    game.HiddenCount = (row * col);

    game.started = 0, game.score = 0;

    for(int i = 0; i < 40; i++)
        for(int j = 0; j < 20; j++)
            game.flag[i][j] = 0, game.bomb[i][j] = 0, 
                game.show[i][j] = 0, game.TableNumbers[i][j] = 0;

    ConstructTable(game.TableSize[0], game.TableSize[1]);
    StartGame();
}

void SetFlag(int x, int y){
    //is it an unrevealed cell, and has the game started yet?
    if(!game.show[(x - 2) / 4][(y - 1) / 2] && game.started){

        //is it already flagged?
        if(!game.flag[(x - 2) / 4][(y - 1) / 2] && game.FlagCount){
            Color(4, 0);
            gotoxy(x, y, "F");
            game.flag[(x - 2) / 4][(y - 1) / 2] = true;
            game.show[(x - 2) / 4][(y - 1) / 2] = true;
            game.FlagCount--;
            Color(7, 0);
        }
        else{
            gotoxy(x, y, " ");
            game.flag[(x - 2) / 4][(y - 1) / 2] = false;
            game.show[(x - 2) / 4][(y - 1) / 2] = false;
            game.FlagCount++;
        }

        gotoxy(0, 2 * (game.TableSize[1] + 1));
        cout<<"Flags Remaining: "<<game.FlagCount<<" "<<endl;
        gotoxy(x, y);
    }
}

void EndGame(bool win, string filler, string result){
    Color(4, 0);

    for(int i = 0; i < game.TableSize[0]; i++)
        for(int j = 0; j < game.TableSize[1]; j++)
            if(game.bomb[i][j])
                gotoxy(4 * i + 2, 2 * j + 1, filler);
                
    Color(7, 0);
    gotoxy(0, 2 * (game.TableSize[1] + 1));

    cout<<"You "<<result<<"!!!            \n";
    cout<<"Score: "<<score() * 2<<'\n';
    cout<<"Time: "<<game.FullTime<<endl;    
    cout<<"Enter Name: ";
    getline(cin, game.PlayerName);

    UpdateScore(game.PlayerName, game.score, game.FullTime);
    Menu();
}

void create(){
    fstream data;
    data.open("score.txt", ios::in);
    if(!data){
        data.open("score.txt", ios::out);
        data<<0<<'\n';
    }
    data.close();
}

void ShowScore(){
    system("cls");
    
    fstream data;
    string name;
    data.open("score.txt", ios::in);
    getline(data, name);
    
    int score, time, count = 1;
    
    cout<<"Top 20 Players\n\n";
    cout<<"Rank     Score     Time     Player\n\n";
    while(data>>name>>score>>time && count <= 20){
        Color((count % 16 ? count % 16 : 1), 0);

		cout<<setw(2)<<count<<setw(12)<<score<<setw(9)<<time;
        gotoxy(28, count + 3, name + '\n');

		count++;
	}
	data.close();
    
    Color(7, 0);
    cout<<"\n\n\nPress any key to continue...";
    
    char c = getch();
	Menu();
}

void UpdateScore(string name, int score, int time){
    ifstream file("score.txt");
    
    int count = 0, exist = 0;
    
    if(file.is_open())
        file>>count;

    string names[count + 1];
    int times[count + 1], scores[count + 1];

    for(int i = 0; i < count; i++)
        file>>names[i]>>scores[i]>>times[i];
    
    for(int i = 0; i < count && !exist; i++)
        if(names[i] == name && scores[i] > score)
                scores[i] = score, times[i] = time, exist = 1,
                names[count] = "";

    if(!exist)
        names[count] = name, scores[count] = score, times[count] = time, count++;

    for(int i = 0; i < count - 1; i++){
        for(int j = 0; j < count - i - 1; j++){
            if(scores[j] < scores[j + 1]){
                swap(names[j], names[j + 1]);
                swap(scores[j], scores[j + 1]);
                swap(times[j], times[j + 1]);
            }
            else if(scores[j] == scores[j + 1] && times[j] > times[j + 1]){
                swap(names[j], names[j + 1]);
                swap(scores[j], scores[j + 1]);
                swap(times[j], times[j + 1]);
            }
        }
    }

    ofstream out("score.txt");
    out<<count<<"\n";
    for(int i = 0; i < count; i++)
        out<<names[i]<<" "<<scores[i]<<" "<<times[i]<<"\n";
    out.close();
}

int score(){
    game.FullTime = (clock() - game.StartTime) / CLOCKS_PER_SEC;
    game.score += (500) / max(10, int(game.FullTime));

    for(int i = 0; i < game.TableSize[0]; i++)
        for(int j = 0; j < game.TableSize[1]; j++)
            if(game.show[i][j] && game.TableNumbers[i][j] > 0)
                game.score += game.TableNumbers[i][j];

    return game.score;
}

void Solve(){
    for(int i = 0; i < game.TableSize[0]; i++)
        for(int j = 0; j < game.TableSize[1]; j++)
            if(Check(i, j))
                Solve();
}

int Check(int x, int y){
    int change = 0;

    if(!game.show[x][y])
        return 0;
    
    //flagging
    int empty = 0, flag = 0;
    pair <int, int> adj[8] = {{x - 1, y - 1}, {x - 1, y}, {x - 1, y + 1}, {x, y - 1}, {x, y + 1}, {x + 1, y - 1}, {x + 1, y}, {x + 1, y + 1}};
    for(int i = 0; i < 8; i++){
        if(adj[i].first < 0 || adj[i].second < 0 || adj[i].first >= game.TableSize[0] || adj[i].second >= game.TableSize[1])
            continue;
        empty += (!game.show[adj[i].first][adj[i].second]);
        flag += (game.flag[adj[i].first][adj[i].second]);
    }
    if(game.TableNumbers[x][y] - flag == empty){        
        for(int i = 0; i < 8; i++){
            if(adj[i].first < 0 || adj[i].second < 0 || adj[i].first >= game.TableSize[0] || adj[i].second >= game.TableSize[1])
                continue;
            if(!game.show[adj[i].first][adj[i].second]){
                SetFlag(adj[i].first * 4 + 2, adj[i].second * 2 + 1);
                change = 1;
            }
        }
    }
    if(!(game.TableNumbers[x][y] - flag)){
        for(int i = 0; i < 8; i++){
            if(adj[i].first < 0 || adj[i].second < 0 || adj[i].first >= game.TableSize[0] || adj[i].second >= game.TableSize[1])
                continue;   
            if(!game.show[adj[i].first][adj[i].second]){
                unroll(adj[i].first, adj[i].second);
                change = 1;
            }
        }   
    }
    return change;
}
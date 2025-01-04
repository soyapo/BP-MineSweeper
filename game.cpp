#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>

using namespace std;

#define fst ios_base::sync_with_stdio(0); cin.tie(NULL);

//--------------------------

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

//--------------------------

struct{
    int pos[2] = {1, 2},
    TableSize[2],
    TableNumbers[20][40];
    
    clock_t StartTime, GameTime;

    int BombCount, FlagCount, HiddenCount, score;

    bool started, bomb[20][40], flag[20][40], show[20][40];

    string PlayerName,
    options[4] = {
        "New Game",
        "How To Play",
        "Leaderboard",
        "Exit"
    },
    difficulties[4] = {
        "Easy",
        "Medium",
        "Hard",
        "Back"
    };

} game;

//--------------------------

void gotoxy(short x, short y, string s = "");

void Color(int f, int b);
void Logo();
void PrintChar(string s, int n);

void ConstructTable(int row, int column);
void SetTableNumbers(int height, int width, int bombcount, int x, int y);

void StartGame();
void unroll(int x, int y);

void Menu();
void Instruction();
void ButtonSwitch(int index, int list_index);

void SetDifficulty();
void SetValue(int row, int col, int bomb);
void SetFlag();

void EndGame(bool win);


void Create();
void ShowScore();
void UpdateScore(string name, int score, int time);

int score();

//--------------------------

int main(){
    Menu();

    return 0;
}

void gotoxy(short x, short y, string s){
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

void ConstructTable(int row, int column){
    system("cls");

    cout<<TopLeftCorner;
    
    for(int i = 0; i < column - 1; i++){
        PrintChar(HorizontalBar, 3);
        cout<<ThreeWayDown;
    }

    PrintChar(HorizontalBar, 3);
    cout<<TopRightCorner<<endl;
    
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

    cout<<endl<<BottomLeftCorner;
    
    for(int i = 0; i < column - 1; i++){
        PrintChar(HorizontalBar, 3);
        cout<<ThreeWayUp;
    }
    
    PrintChar(HorizontalBar, 3);
    cout<<BottomRightCorner;

    cout<<endl<<endl<<"Flags Remaining: "<<game.FlagCount<<endl<<endl;

    gotoxy(game.pos[1], game.pos[0]);
}

void SetTableNumbers(int height, int width, int bombcount, int x, int y){
    game.TableNumbers[x][y] = 0;
    
    int RandX, RandY;
    
    srand(time(0));
    while(bombcount){
        RandX = rand() % height;
        RandY = rand() % width;
        if(abs(RandX - x) > 1 && abs(RandY - y) > 1 && !game.bomb[RandX][RandY])
            game.bomb[RandX][RandY] = true, game.TableNumbers[RandX][RandY] = -1, bombcount--;
    }
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
    while(true){
        move = getch();
        switch(move){
        case 'w':
            if(game.pos[0] != 1){
                game.pos[0] += -2;
                gotoxy(game.pos[1], game.pos[0]);
            }
            break;
        case 's':
            if(game.pos[0] != 2 * game.TableSize[0] - 1){
                game.pos[0] += 2;
                gotoxy(game.pos[1], game.pos[0]);
            }
            break;
        case 'a':
            if(game.pos[1] != 2){
                game.pos[1] += -4;
                gotoxy(game.pos[1], game.pos[0]);
            }
            break;
        case 'd':
            if(game.pos[1] != 4 * game.TableSize[1] - 2){
                game.pos[1] += 4;
                gotoxy(game.pos[1], game.pos[0]);
            }
            break;
        case 'f':
            SetFlag();
            break;
        case 'c':
            if(!game.started){
                SetTableNumbers(game.TableSize[0], game.TableSize[1], game.BombCount, (game.pos[0] - 1) / 2, (game.pos[1] - 2) / 4);
                gotoxy(game.pos[1], game.pos[0]);

                game.started = 1;
                game.StartTime = clock();
            }
            else if(!game.flag[(game.pos[0] - 1) / 2][(game.pos[1] - 2) / 4]){
                unroll((game.pos[0] - 1) / 2, (game.pos[1] - 2) / 4);
                gotoxy(game.pos[1], game.pos[0]);
            }

            break;
        default:
            break;
        }
    }
}

void unroll(int x, int y){
    if(game.show[x][y])
        return;

    if(!game.TableNumbers[x][y]){
        Color(7, 0);
        gotoxy(4 * y + 2, 2 * x + 1, "-");
        game.show[x][y] = true, game.HiddenCount--;

        if(game.HiddenCount == game.BombCount)
            EndGame(1);

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
    else if(game.TableNumbers[x][y] > 0){
        Color(game.TableNumbers[x][y], 0);
        gotoxy(4 * y + 2, 2 * x + 1, to_string(game.TableNumbers[x][y]));
        game.show[x][y] = true, game.HiddenCount--;
        Color(7, 0);
        
        if(game.HiddenCount == game.BombCount)
            EndGame(1);
        
        return;
    }
    else if(game.TableNumbers[x][y] == -1){
        EndGame(0);
    }
}

void Menu(){
    int index = 0;
    ButtonSwitch(index, 0);
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
                    case 0: SetDifficulty();
                    case 1: Instruction();
                    case 2: ShowScore();
                    case 3: exit(0);
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

    for(int i = 0; i < 4; i++){
        cout<<"          ";

        if(i == 3 && list_index == 1)
            cout<<"\n          ";

        cout<<(i == index ? "-> " : "   ");
        cout<<(list_index? game.difficulties[i] : game.options[i]);
        cout<<(i == index ? " <-" : "")<<endl;
    }
}

void SetDifficulty(){
    int index = 0;
    ButtonSwitch(index, 1);
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
    int pos[2] = {1, 2};

    game.TableSize[0] = row, game.TableSize[1] = col;
    game.BombCount = bomb;
    game.FlagCount = bomb;
    game.HiddenCount = (row * col);

    game.started = 0, game.score = 0;

    for(int i = 0; i < 20; i++)
        for(int j = 0; j < 40; j++)
            game.flag[i][j] = 0, game.bomb[i][j] = 0, 
                game.show[i][j] = 0, game.TableNumbers[i][j] = 0;

    ConstructTable(game.TableSize[0], game.TableSize[1]);
    StartGame();
}

void SetFlag(){
    if(!game.show[(game.pos[0] - 1) / 2][(game.pos[1] - 2) / 4] && game.started){

        if(!game.flag[(game.pos[0] - 1) / 2][(game.pos[1] - 2) / 4] && game.FlagCount){
            Color(4, 0);
            cout<<"F";
            game.flag[(game.pos[0] - 1) / 2][(game.pos[1] - 2) / 4] = true;
            game.FlagCount--;
            Color(7, 0);
        }
        else{
            cout<<" ";
            game.flag[(game.pos[0] - 1) / 2][(game.pos[1] - 2) / 4] = false;
            game.FlagCount++;
        }

        gotoxy(0, 2 * (game.TableSize[0] + 1));
        cout<<"Flags Remaining: "<<game.FlagCount<<" "<<endl;
        gotoxy(game.pos[1], game.pos[0]);
    }
}

void EndGame(bool win){

    if(win){
        for(int i = 0; i < game.TableSize[0]; i++){
            for(int j = 0; j < game.TableSize[1]; j++){
                if(game.bomb[i][j]){
                    Color(4, 0);
                    gotoxy(4 * j + 2, 2 * i + 1, "F");
                    Color(7, 0);
                }
            }
        }

        gotoxy(0, 2 * (game.TableSize[0] + 1));

        cout<<"You Won!!!            \n";
        cout<<"Score: "<<score() * 2<<'\n';
        cout<<"Time: "<<game.GameTime<<endl;
    }

    else{
        for(int i = 0; i < game.TableSize[0]; i++){
            for(int j = 0; j < game.TableSize[1]; j++){
                if(game.bomb[i][j]){
                    Color(4, 0);
                    gotoxy(4 * j + 2, 2 * i + 1, "*");
                    Color(7, 0);
                }
            }
        }

        gotoxy(0, 2 * (game.TableSize[0] + 1));

        cout<<"You Lost!!!            \n\n";
        cout<<"Score: "<<score()<<'\n';
        cout<<"Time: "<<game.GameTime<<endl;
    }
    
    cout<<"Enter Name: ";
    getline(cin, game.PlayerName);
    
    UpdateScore(game.PlayerName, game.score, game.GameTime);
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
    data.open("score.txt", ios::in);
    
    string name;
    int score, time, count = 1;
    
    getline(data, name);
    
    cout<<"Top 20 Players\n\n";
    cout<<"Rank     Score     Time     Player\n\n";
    while(data>>name>>score>>time){
        Color(count, 0);
		cout<<setw(2)<<count;
		cout<<setw(12)<<score;
		cout<<setw(9)<<time;
        gotoxy(28, count + 3, name);
        cout<<endl;

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
    game.GameTime = (clock() - game.StartTime) / CLOCKS_PER_SEC;
    game.score += (500) / max(10, int(game.GameTime));

    for(int i = 0; i < game.TableSize[0]; i++)
        for(int j = 0; j < game.TableSize[1]; j++)
            if(game.show[i][j] && game.TableNumbers[i][j] > 0)
                game.score += game.TableNumbers[i][j];

    return game.score;
}
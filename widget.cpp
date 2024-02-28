#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include "widget.h"
#include "ui_widget.h"
#include<QDebug>

int item1[4][4]=
    {
        {0,0,0,0},
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0}
};

int item2[4][4]=
    {
        {0,1,0,0},
        {0,1,0,0},
        {0,1,1,0},
        {0,0,0,0}
};

int item3[4][4]=
    {
        {0,0,1,0},
        {0,0,1,0},
        {0,1,1,0},
        {0,0,0,0}
};

int item4[4][4]=
    {
        {0,1,0,0},
        {0,1,1,0},
        {0,0,1,0},
        {0,0,0,0}
};

int item5[4][4]=
    {
        {0,0,1,0},
        {0,1,1,0},
        {0,1,0,0},
        {0,0,0,0}
};

int item6[4][4]=
    {
        {0,0,0,0},
        {0,0,1,0},
        {0,1,1,1},
        {0,0,0,0}
};

int item7[4][4]=
    {
        {0,0,1,0},
        {0,0,1,0},
        {0,0,1,0},
        {0,0,1,0}
};

int item8[4][4]=
    {
        {0,1,0,0},
        {0,1,1,0},
        {0,0,1,0},
        {0,0,0,0}
};
inline void block_cpy(int dblock[4][4],int sblock[4][4])
{
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            dblock[i][j]=sblock[i][j];
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize(AREA_COL*BLOCK_SIZE+MARGIN*3+5*BLOCK_SIZE,AREA_ROW*BLOCK_SIZE+MARGIN*3);
    InitGame();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    // Draw game background
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(MARGIN, MARGIN, AREA_COL * BLOCK_SIZE, AREA_ROW * BLOCK_SIZE);

    // Draw next block preview
    painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (next_block[i][j] == 1) {
                painter.drawRect(MARGIN * 3 + AREA_COL * BLOCK_SIZE + j * BLOCK_SIZE, MARGIN + i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
            }
        }
    }

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 14));
    // Display level
    painter.drawText(QRect(MARGIN * 3 + AREA_COL * BLOCK_SIZE, MARGIN * 2 + 7 * BLOCK_SIZE, BLOCK_SIZE * 4, BLOCK_SIZE), Qt::AlignCenter, "Level: " + QString::number(level));
    // Display score
    painter.drawText(QRect(MARGIN * 3 + AREA_COL * BLOCK_SIZE, MARGIN + 9 * BLOCK_SIZE, BLOCK_SIZE * 4, BLOCK_SIZE * 4), Qt::AlignCenter, "score: " + QString::number(score));
    // Draw background for the score line (optional, for better visibility)
    painter.setBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
    painter.drawRect(scoreLineX, scoreLineY, scoreLineWidth, scoreLineHeight);

    // Calculate the length of the filled part of the score line
    int filledLength = static_cast<int>((static_cast<double>(score) / nextLevelScore) * scoreLineWidth);
    painter.setBrush(QBrush(Qt::darkGreen, Qt::SolidPattern)); // Choose a color that fits your game's theme
    painter.drawRect(scoreLineX, scoreLineY, filledLength, scoreLineHeight);

    // Draw blocks on game board
    for (int i = 0; i < AREA_ROW; i++) {
        for (int j = 0; j < AREA_COL; j++) {
            if (game_area[i][j] == 1) {
                painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
                painter.drawRect(j * BLOCK_SIZE + MARGIN, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
            } else if (game_area[i][j] == 2) {
                painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
                painter.drawRect(j * BLOCK_SIZE + MARGIN, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
            }
        }
    }
}


void Widget::timerEvent(QTimerEvent *event)
{

    if(event->timerId()==game_timer)
        BlockMove(DOWN);

    if(event->timerId()==paint_timer)
        update();
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        BlockMove(UP);
        break;
    case Qt::Key_Down:
        BlockMove(DOWN);
        break;
    case Qt::Key_Left:
        BlockMove(LEFT);
        break;
    case Qt::Key_Right:
        BlockMove(RIGHT);
        break;
    case Qt::Key_Space:
        BlockMove(SPACE);
        break;
    default:
        break;
    }
}

void Widget::CreateBlock(int block[4][4],int block_id)
{
    switch (block_id)
    {
    case 0:
        block_cpy(block,item1);
        break;
    case 1:
        block_cpy(block,item2);
        break;
    case 2:
        block_cpy(block,item3);
        break;
    case 3:
        block_cpy(block,item4);
        break;
    case 4:
        block_cpy(block,item5);
        break;
    case 5:
        block_cpy(block,item6);
        break;
    case 6:
        block_cpy(block,item7);
        break;
    case 7:
        block_cpy(block,item8);
        break;
    default:
        break;
    }
}

void Widget::GetBorder(int block[4][4],Border &border)
{
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(block[i][j]==1)
            {
                border.dbound=i;
                break;
            }
    for(int i=3;i>=0;i--)
        for(int j=0;j<4;j++)
            if(block[i][j]==1)
            {
                border.ubound=i;
                break;
            }
    for(int j=0;j<4;j++)
        for(int i=0;i<4;i++)
            if(block[i][j]==1)
            {
                border.rbound=j;
                break;
            }
    for(int j=3;j>=0;j--)
        for(int i=0;i<4;i++)
            if(block[i][j]==1)
            {
                border.lbound=j;
                break;
            }
}

void Widget::InitGame()
{
    // Clear the game area
    for(int i = 0; i < AREA_ROW; i++)
        for(int j = 0; j < AREA_COL; j++)
            game_area[i][j] = 0;

    // Set game speed and refresh rates
    speed_ms = 800; // Time in milliseconds before the block moves down one row
    refresh_ms = 30; // Time in milliseconds for how often the screen refreshes

    // Initialize the random number generator
    srand(time(0));

    // Reset the score
    score = 0;

    // Generate the first and next blocks
    int block_id = rand() % BLOCKS_NUM; // Randomly select the first block
    CreateBlock(next_block, block_id); // Prepare the next block

    // Start the game
    StartGame();
}

void Widget::updateScore(int scoreToAdd) {
    score += scoreToAdd;
    if (score >= 100) {
        level++;
        nextLevelScore += 20 * level; // Increase the threshold for the next level
        speed_ms = std::max(speed_ms - 50, 100); // Example: Increase game speed for each level up, with a lower limit
        killTimer(game_timer); // Restart the timer with the new speed
        game_timer = startTimer(speed_ms);
    }
}


void Widget::ResetBlock()
{
    block_cpy(cur_block,next_block);
    GetBorder(cur_block,cur_border);

    int block_id=rand()%BLOCKS_NUM;
    CreateBlock(next_block,block_id);

    block_point start_point;
    start_point.pos_x=AREA_COL/2-2;
    start_point.pos_y=0;
    block_pos=start_point;
}

void Widget::StartGame()
{
    game_timer=startTimer(speed_ms);
    paint_timer=startTimer(refresh_ms);
    int block_id=rand()%BLOCKS_NUM;
    CreateBlock(next_block,block_id);
    ResetBlock();
}

void Widget::GameOver()
{
    killTimer(game_timer);
    killTimer(paint_timer);
    QMessageBox::information(this,"failed","game over");
}

void Widget::BlockRotate(int block[4][4])
{
    int temp_block[4][4];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            temp_block[3-j][i]=block[i][j];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            block[i][j]=temp_block[i][j];
}

void Widget::ConvertStable(int x,int y)
{
    for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
        for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
            if(cur_block[i][j]==1)
                game_area[y+i][x+j]=2;
}

bool Widget::IsCollide(int x, int y, Direction dir) {
    // Temporary block for rotation
    int tempBlock[4][4];
    block_cpy(tempBlock, cur_block);

    // Rotate block if direction is UP
    if (dir == UP) BlockRotate(tempBlock);

    // Adjust x and y based on direction
    switch (dir) {
    case LEFT: x -= 1; break;
    case RIGHT: x += 1; break;
    case DOWN: y += 1; break;
    default: break;
    }

    // Check collision with game area boundaries and other blocks
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (tempBlock[i][j] == 0) continue;
            int newX = x + j, newY = y + i;

            // Boundary checks
            if (newX < 0 || newX >= AREA_COL || newY >= AREA_ROW){
                qInfo( "iscollide blocks x" );
                return true;
            }

            if (newY >= 0 && game_area[newY][newX] == 2){
                qInfo( "iscollide blocks y" );
                return true;
            }
        }
    }
    return false;
}

void Widget::ClearBlockPosition(int block[4][4], int x, int y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block[i][j] == 1 && (y + i >= 0 && y + i < AREA_ROW) && (x + j >= 0 && x + j < AREA_COL)) {
                // Тільки якщо ця частина блоку належить до активного блоку, очищаємо її
                if (game_area[y + i][x + j] == 1) {
                    game_area[y + i][x + j] = 0;
                }
            }
        }
    }
}

void Widget::PlaceBlock(int block[4][4], int x, int y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block[i][j] == 1 && (y + i >= 0 && y + i < AREA_ROW) && (x + j >= 0 && x + j < AREA_COL)) {
                game_area[y + i][x + j] = 1; // Або інше значення, яке використовується для активного блоку
            }
        }
    }
}


void Widget::BlockMove(Direction dir)
{
    switch (dir) {
    case UP:
        int tempBlock[4][4];
        block_cpy(tempBlock, cur_block);

        // Обертання тимчасового блоку
        BlockRotate(tempBlock);

        // Перевірка на колізії після обертання
        if (!IsCollide(block_pos.pos_x, block_pos.pos_y, UP)) {
            // Якщо колізій немає, очистити попереднє положення блоку на ігровому полі
            ClearBlockPosition(cur_block, block_pos.pos_x, block_pos.pos_y);

            // Копіювання оберненого блоку назад до поточного блоку
            block_cpy(cur_block, tempBlock);

            // Оновлення межі блоку після обертання
            GetBorder(cur_block, cur_border);

            // Поміщення блоку в нове положення
            PlaceBlock(cur_block, block_pos.pos_x, block_pos.pos_y);
        }
        break;

    case DOWN:

        if(block_pos.pos_y+cur_border.dbound==AREA_ROW-1)
        {
            ConvertStable(block_pos.pos_x,block_pos.pos_y);
            ResetBlock();
            break;
        }

        if(IsCollide(block_pos.pos_x,block_pos.pos_y,DOWN))
        {
            ConvertStable(block_pos.pos_x,block_pos.pos_y);
            ResetBlock();
            break;
        }

        for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
            game_area[block_pos.pos_y][block_pos.pos_x+j]=0;

        block_pos.pos_y+=1;
        for(int i=0;i<4;i++)
            for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
                if(block_pos.pos_y+i<=AREA_ROW-1&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2)
                    game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
        break;
    case LEFT:

        if(block_pos.pos_x+cur_border.lbound==0||IsCollide(block_pos.pos_x,block_pos.pos_y,LEFT))
            break;
        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
            game_area[block_pos.pos_y+i][block_pos.pos_x+3]=0;
        block_pos.pos_x-=1;
        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
            for(int j=0;j<4;j++)
                if(block_pos.pos_x+j>=0&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2)
                    game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
        break;
    case RIGHT:
        if(block_pos.pos_x+cur_border.rbound==AREA_COL-1||IsCollide(block_pos.pos_x,block_pos.pos_y,RIGHT))
            break;

        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
            game_area[block_pos.pos_y+i][block_pos.pos_x]=0;
        block_pos.pos_x+=1;

        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
            for(int j=0;j<4;j++)
                if(block_pos.pos_x+j<=AREA_COL-1&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2)
                    game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
        break;
    case SPACE:

        while(block_pos.pos_y+cur_border.dbound<AREA_ROW-1&&!IsCollide(block_pos.pos_x,block_pos.pos_y,DOWN))
        {
            for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
                game_area[block_pos.pos_y][block_pos.pos_x+j]=0;

            block_pos.pos_y+=1;

            for(int i=0;i<4;i++)
                for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
                    if(block_pos.pos_y+i<=AREA_ROW-1&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2)
                        game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
        }
        ConvertStable(block_pos.pos_x,block_pos.pos_y);
        ResetBlock();
        break;
    default:
        break;
    }

    int i=AREA_ROW-1;
    int line_count=0;
    while(i>=1)
    {
        bool is_line_full=true;
        for(int j=0;j<AREA_COL;j++)
            if(game_area[i][j]==0)
            {
                is_line_full=false;
                i--;
                break;
            }
        if(is_line_full)
        {
            for(int k=i;k>=1;k--)
                for(int j=0;j<AREA_COL;j++)
                    game_area[k][j]=game_area[k-1][j];
            line_count++;
        }
    }
    if (line_count > 0) {
        score += line_count * 10; // Припустимо, що за кожен очищений ряд дається 10 очок.
        updateScore(line_count * 10); // Оновлення рахунку гравця з урахуванням нових очок.
    }
    for(int j=0;j<AREA_COL;j++)
        if(game_area[0][j]==2)
            GameOver();
}






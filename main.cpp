#include <simplecpp>
#include <sstream>
#include "shooter.h"
#include "bubble.h"

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);
char score = '0';
float timer = 120;
char lives = '3';
bool phase=false;


void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void collisionbb(vector<Bubble> &bubbles, vector<Bullet> &bullets){
    //making the bubble and bullet disappear on collision
    for (unsigned int i=0; i<bullets.size(); i++){
        for (unsigned int j=0; j<bubbles.size(); j++){
            int dist = sqrt( pow(bullets[i].get_center_x() - bubbles[j].get_center_x(), 2.0) + pow(bullets[i].get_center_y() - bubbles[j].get_center_y(), 2.0));
            if (dist<= (bubbles[j].get_radius()+sqrt(2)*bullets[i].get_width()/2)){
                bubbles.erase(bubbles.begin()+j);
                bullets.erase(bullets.begin()+i);
                if (score<='8') score+=1;
                return;
            }
        }
    }
}

void collisionbb1(vector<Bubble> &bubbles1, vector<Bullet> &bullets, vector<Bubble> &bubbles){
    //making the bubble split into 2 small bubbles and bullet disappear on collision
    for (unsigned int i=0; i<bullets.size(); i++){
        for (unsigned int j=0; j<bubbles1.size(); j++){
            int dist = sqrt( pow(bullets[i].get_center_x() - bubbles1[j].get_center_x(), 2.0) + pow(bullets[i].get_center_y() - bubbles1[j].get_center_y(), 2.0));
            if (dist<= (bubbles1[j].get_radius()+sqrt(2)*bullets[i].get_width()/2)){
                bubbles.push_back(Bubble(bubbles1[j].get_center_x(), bubbles1[j].get_center_y(), BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, bubbles1[j].get_vy(), BUBBLE_DEFAULT_AY, bubbles1[j].get_color()));
                bubbles.push_back(Bubble(bubbles1[j].get_center_x(), bubbles1[j].get_center_y(), BUBBLE_DEFAULT_RADIUS, -1.0*BUBBLE_DEFAULT_VX, bubbles1[j].get_vy(), BUBBLE_DEFAULT_AY, bubbles1[j].get_color()));
                bubbles1.erase(bubbles1.begin()+j);
                bullets.erase(bullets.begin()+i);
            }
        }
    }
}

void collisionbb2(vector<Bubble> &bubbles2, vector<Bullet> &bullets, vector<Bubble> &bubbles1){
    //making the bubble split into 2 medium bubbles and bullet disappear on collision
    for (unsigned int i=0; i<bullets.size(); i++){
        for (unsigned int j=0; j<bubbles2.size(); j++){
            int dist = sqrt( pow(bullets[i].get_center_x() - bubbles2[j].get_center_x(), 2.0) + pow(bullets[i].get_center_y() - bubbles2[j].get_center_y(), 2.0));
            if (dist<= (bubbles2[j].get_radius()+sqrt(2)*bullets[i].get_width()/2)){
                bubbles1.push_back(Bubble(bubbles2[j].get_center_x(), bubbles2[j].get_center_y(), BUBBLE_DEFAULT_RADIUS*sqrt(2), BUBBLE_DEFAULT_VX, bubbles2[j].get_vy(), BUBBLE_DEFAULT_AY, bubbles2[j].get_color()));
                bubbles1.push_back(Bubble(bubbles2[j].get_center_x(), bubbles2[j].get_center_y(), BUBBLE_DEFAULT_RADIUS*sqrt(2), -1.0*BUBBLE_DEFAULT_VX, bubbles2[j].get_vy(), BUBBLE_DEFAULT_AY, bubbles2[j].get_color()));
                bubbles2.erase(bubbles2.begin()+j);
                bullets.erase(bullets.begin()+i);
            }
        }
    }
}

bool collisionbs(vector<Bubble> &bubbles, Shooter &shooter){
    //making the shooter die if it gets hit by a bullet
    for (unsigned int i=0; i<bubbles.size(); i++){
        int dist1 = sqrt( pow(shooter.get_head_center_x() - bubbles[i].get_center_x(), 2.0) + pow(shooter.get_head_center_y() - bubbles[i].get_center_y(), 2.0));
        if (dist1 <= (bubbles[i].get_radius()+shooter.get_head_radius())){
            if(!phase){
                phase=true;
                lives-=1;
            }
            return true;
        }
        int dist2 = abs(bubbles[i].get_center_x() - shooter.get_body_center_x());
        int dist3 = abs(bubbles[i].get_center_y() - shooter.get_body_center_y());
        if ((dist2 <= (bubbles[i].get_radius() + shooter.get_body_width()/2.0)) && (dist3 <= (bubbles[i].get_radius() + shooter.get_body_height()/2.0))){
            if(!phase){
                phase=true;
                lives-=1;
            }
            return true;
        }
    }
    return false;
}

bool collisionbs1(vector<Bubble> &bubbles1, Shooter &shooter){
    //making the shooter die if it gets hit by a bullet
    for (unsigned int i=0; i<bubbles1.size(); i++){
        int dist1 = sqrt( pow(shooter.get_head_center_x() - bubbles1[i].get_center_x(), 2.0) + pow(shooter.get_head_center_y() - bubbles1[i].get_center_y(), 2.0));
        if (dist1 <= (bubbles1[i].get_radius()+shooter.get_head_radius())){
            if(!phase){
                phase=true;
                lives-=1;
            }
            return true;
        }
        int dist2 = abs(bubbles1[i].get_center_x() - shooter.get_body_center_x());
        int dist3 = abs(bubbles1[i].get_center_y() - shooter.get_body_center_y());
        if ((dist2 <= (bubbles1[i].get_radius() + shooter.get_body_width()/2.0)) && (dist3 <= (bubbles1[i].get_radius() + shooter.get_body_height()/2.0))){
            if(!phase){
                phase=true;
                lives-=1;
            }
            return true;
        }
    }
    return false;
}

bool collisionbs2(vector<Bubble> &bubbles2, Shooter &shooter){
    //making the shooter die if it gets hit by a bullet
    for (unsigned int i=0; i<bubbles2.size(); i++){
        int dist1 = sqrt( pow(shooter.get_head_center_x() - bubbles2[i].get_center_x(), 2.0) + pow(shooter.get_head_center_y() - bubbles2[i].get_center_y(), 2.0));
        if (dist1 <= (bubbles2[i].get_radius()+shooter.get_head_radius())){
            if(!phase){
                phase=true;
                lives-=1;
            }
            return true;
        }
        int dist2 = abs(bubbles2[i].get_center_x() - shooter.get_body_center_x());
        int dist3 = abs(bubbles2[i].get_center_y() - shooter.get_body_center_y());
        if ((dist2 <= (bubbles2[i].get_radius() + shooter.get_body_width()/2.0)) && (dist3 <= (bubbles2[i].get_radius() + shooter.get_body_height()/2.0))){
            if(!phase){
                phase=true;
                lives-=1;
            }
            return true;
        }
    }
    return false;
}

vector<Bubble> create_bubbles()
{
    // create initial small bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, BUBBLE_DEFAULT_AY, COLOR(255,105,180)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, BUBBLE_DEFAULT_AY, COLOR(255,105,180)));
    return bubbles;
}

vector<Bubble> create_bubbles1()
{
    // create initial medium bubbles in the game
    vector<Bubble> bubbles1;
    bubbles1.push_back(Bubble(WINDOW_X/2.5, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*sqrt(2), -BUBBLE_DEFAULT_VX, 0, BUBBLE_DEFAULT_AY, COLOR(0,255,255)));
    bubbles1.push_back(Bubble(WINDOW_X/5.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*sqrt(2), BUBBLE_DEFAULT_VX, 0, BUBBLE_DEFAULT_AY, COLOR(0,255,255)));
    return bubbles1;
}

vector<Bubble> create_bubbles2()
{
    // create initial big bubbles in the game
    vector<Bubble> bubbles2;
    bubbles2.push_back(Bubble(WINDOW_X/1.5, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*2, -BUBBLE_DEFAULT_VX, 0, BUBBLE_DEFAULT_AY, COLOR(200,162,200)));
    return bubbles2;
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

void move_bubbles1(vector<Bubble> &bubbles1){
    // move all bubbles
    for (unsigned int i=0; i < bubbles1.size(); i++)
    {
        bubbles1[i].nextStep(STEP_TIME);
    }
}

void move_bubbles2(vector<Bubble> &bubbles2){
    // move all bubbles
    for (unsigned int i=0; i < bubbles2.size(); i++)
    {
        bubbles2[i].nextStep(STEP_TIME);
    }
}


int main()
{
    bool shootercolorchange=false;
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    string msg_score("Score: _");
    Text scoree(250, BOTTOM_MARGIN, msg_score);

    string msg_lives("Lives: _");
    Text livess(450, BOTTOM_MARGIN,msg_lives);

    string msg_timer("Timer:    ");
    Text timerr(LEFT_MARGIN, 20, msg_timer);

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles();
    vector<Bubble> bubbles1 = create_bubbles1();
    vector<Bubble> bubbles2 = create_bubbles2();

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        //Update the score
        msg_score[msg_score.length()-1] = score;
        scoree.setMessage(msg_score);

        //Update the lives
        msg_lives[msg_lives.length()-1] = lives;
        livess.setMessage(msg_lives);

        // Update the bubbles
        move_bubbles(bubbles);
        move_bubbles1(bubbles1);
        move_bubbles2(bubbles2);

        // Update the bullets
        move_bullets(bullets);

        //timer
        timer-=0.03;
        stringstream sstimer;
        sstimer<<int(timer);
        string stimer;
        sstimer>>stimer;
        for (int i=stimer.length()-1;i>=0;i--){
             if (timer>=100) msg_timer[msg_timer.length()-3+i] = stimer[i];
             else if (timer>=9) {
                msg_timer[msg_timer.length()-3] = '0';
                msg_timer[msg_timer.length()-2+i] = stimer[i];
             }
             else {
                msg_timer[msg_timer.length()-2] = '0';
                msg_timer[msg_timer.length()-1+i] = stimer[i];
             }
        }
        timerr.setMessage(msg_timer);

        //calling the bubble-bullet collisions
        collisionbb(bubbles, bullets);
        collisionbb1(bubbles1, bullets, bubbles);
        collisionbb2(bubbles2, bullets, bubbles1);

        //calling the shooter-bullet collisions
        shootercolorchange=collisionbs(bubbles, shooter)||collisionbs1(bubbles1, shooter)||collisionbs2(bubbles2, shooter);
        if (shootercolorchange==false) phase=false;
        shooter.shooter_color(phase);

        if (bubbles.size()==0 && bubbles1.size()==0 && bubbles2.size()==0){
            Text t(250,225,"You Win!");
            Text t1(250,250,"Score: 10");
            Text t2(250,275,msg_timer);
            wait(5);
            break;
        }

        if (lives=='0'||timer<=0) {
            Text t(250,225,"You Lose!");
            Text t1(250,250,msg_score);
            Text t2(250,275,msg_timer);
            wait(5);
            break;
        }

    }
}


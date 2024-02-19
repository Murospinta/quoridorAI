#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_set>
#include <queue>


using namespace std;
struct Wall{// the order of picking nodes must be the same in between sides
    int nodeSide11;
    int nodeSide12;
    int nodeSide21;
    int nodeSide22;
    bool placed;
    int index;
};

struct Node {
    int player;
    Node* up;
    Node* down;
    Node* left;
    Node* right;

    Node(){
        this->player = 0;   
        this->up = nullptr; 
        this->down = nullptr;
        this->left = nullptr;
        this->right = nullptr;
    }
};

struct Game {
    Node board[81];
    int player1Pos;
    int player2Pos;
    Wall walls[128];
    int player1Walls;
    int player2Walls;

    Game() {
        player1Pos = 76;
        player2Pos = 4;
        player1Walls = 10;
        player2Walls = 10;
        for (int i = 0; i < 81; i++) {//creates the board
            if (i % 9 != 0) {
                board[i].left = &board[i - 1];
                board[i].player = 0;
            }
            if ((i + 1) % 9 != 0) {
                board[i].right = &board[i + 1];
                board[i].player = 0;
            }
            if (i >= 9) {
                board[i].up = &board[i - 9];
                board[i].player = 0;
            }
            if (i < 72) {
                board[i].down = &board[i + 9];
                board[i].player = 0;
            }
        }
        board[76].player = 1;
        board[4].player = 2;
        int comp = 0;//compensation for the rightmost nodes that cant form a 4 node square
        for(int i = 0; i < 64; i++){//creates all possible horizontal walls
            if ((i + 1 + comp) % 9 != 0) {
                walls[i].nodeSide11 = i + comp;
                walls[i].nodeSide12 = i + 1 + comp;
                walls[i].nodeSide21 = i + 9 + comp;
                walls[i].nodeSide22 = i + 10 + comp;
                walls[i].index = i;
                walls[i].placed = false;
            }
            else{
                comp++;
                walls[i].nodeSide11 = i + comp;
                walls[i].nodeSide12 = i + 1 + comp;
                walls[i].nodeSide21 = i + 9 + comp;
                walls[i].nodeSide22 = i + 10 + comp;
                walls[i].index = i;
                walls[i].placed = false;
            }
        }
        for(int i = 0; i < 64; i++){//creates all possible vertical walls
            walls[i + 64].nodeSide11 = walls[i].nodeSide11;
            walls[i + 64].nodeSide12 = walls[i].nodeSide21;//flips the sides
            walls[i + 64].nodeSide21 = walls[i].nodeSide12;//flips the sides
            walls[i + 64].nodeSide22 = walls[i].nodeSide22;
            walls[i + 64].index = i + 64;
            walls[i + 64].placed = false;
        }
    }
    
    Game clone(){//copilot... needs to be revised
        Game clone;
        for(int i = 0; i < 81; i++){
            clone.board[i].player = board[i].player;
            clone.board[i].up = board[i].up;
            clone.board[i].down = board[i].down;
            clone.board[i].left = board[i].left;
            clone.board[i].right = board[i].right;
        }
        clone.player1Pos = player1Pos;
        clone.player2Pos = player2Pos;
        clone.player1Walls = player1Walls;
        clone.player2Walls = player2Walls;

        for(int i = 0; i < 128; i++){
            clone.walls[i].nodeSide11 = walls[i].nodeSide11;
            clone.walls[i].nodeSide12 = walls[i].nodeSide12;
            clone.walls[i].nodeSide21 = walls[i].nodeSide21;
            clone.walls[i].nodeSide22 = walls[i].nodeSide22;
            clone.walls[i].placed = walls[i].placed;
            clone.walls[i].index = walls[i].index;
        }
        return clone;
    }
    
    void printWalls(){
        for(int i = 0; i < 128; i++){
            cout << "Parede " << i << " : "<< walls[i].nodeSide11 << " " << walls[i].nodeSide12 << " " << walls[i].nodeSide21 << " " << walls[i].nodeSide22 << endl;
        }
    }
    
    bool isPath(Node& start, Node& target, unordered_set<Node*>& visited) {
        // Check if the start node is the target
        if (&start == &target) {
            return true;
        }

        // Mark the current node as visited
        visited.insert(&start);
        // Recursively check neighbors
        if (start.up && visited.find(start.up) == visited.end() && isPath(*start.up, target, visited)) {
            return true;
        }
        if (start.down && visited.find(start.down) == visited.end() && isPath(*start.down, target, visited)) {
            return true;
        }
        if (start.left && visited.find(start.left) == visited.end() && isPath(*start.left, target, visited)) {
            return true;
        }
        if (start.right && visited.find(start.right) == visited.end() && isPath(*start.right, target, visited)) {
            return true;
        }

        return false;
    }

    bool isPath(Node& start, Node& target) {
        unordered_set<Node*> visited;
        return isPath(start, target, visited);
    }

    int shortestPath(Node& start, Node& end) {
        // Using BFS to find the shortest path
        queue<pair<Node*, int> > q;  // Note the space between '>' and '>'
        unordered_set<Node*> visited;

        q.push(make_pair(&start, 0));

        while (!q.empty()) {
            Node* currentNode = q.front().first;
            int distance = q.front().second;
            q.pop();

            // Check if the current node is the destination
            if (currentNode == &end) {
                return distance;
            }

            // Explore neighbors
            if (currentNode->up && visited.find(currentNode->up) == visited.end()) {
                q.push(make_pair(currentNode->up, distance + 1));
                visited.insert(currentNode->up);
            }
            if (currentNode->down && visited.find(currentNode->down) == visited.end()) {
                q.push(make_pair(currentNode->down, distance + 1));
                visited.insert(currentNode->down);
            }
            if (currentNode->left && visited.find(currentNode->left) == visited.end()) {
                q.push(make_pair(currentNode->left, distance + 1));
                visited.insert(currentNode->left);
            }
            if (currentNode->right && visited.find(currentNode->right) == visited.end()) {
                q.push(make_pair(currentNode->right, distance + 1));
                visited.insert(currentNode->right);
            }
        }

        // If no path is found, return -1 or some other indicator
        return -1;
    }

    bool areNeighbors(const Node& node1, const Node& node2) {
        if(node1.up == &node2 || node1.down == &node2 || node1.left == &node2 || node1.right == &node2){
            return true;
        }
        if(node2.up == &node1 || node2.down == &node1 || node2.left == &node1 || node2.right == &node1){
            return true;
        }
        return false;
    }
    
    bool canBeWall(Wall wall){ //verifies if the nodes form a wall in the specified format
        if((wall.nodeSide11 == (wall.nodeSide12 + 9) && wall.nodeSide21 == (wall.nodeSide22 + 9))){
            return true;
        }
        if(wall.nodeSide11 == (wall.nodeSide12 - 9) && wall.nodeSide21 == (wall.nodeSide22 - 9)){
            return true;
        }
        if(wall.nodeSide11 == (wall.nodeSide12 + 1) && wall.nodeSide21 == (wall.nodeSide22 + 1)){
            return true;
        }
        if(wall.nodeSide11 == (wall.nodeSide12 - 1) && wall.nodeSide21 == (wall.nodeSide22 - 1)){
            return true;
        }
        return false;
    }
    
    bool isWall(Wall wall){ //verifies if there is a wall on said position
        if(wall.placed){
            return true;
        }
        return false;
    }

    bool isPartWall(Wall wall){//verifies if there is a part of a wall on said position
        if(wall.index < 63){
            if (wall.index % 8 == 0) {
                if(walls[wall.index + 1].placed){
                    return true;
                }
            }
            else if ((wall.index + 1) % 8 == 0) {
                if(walls[wall.index - 1].placed){
                    return true;
                }
            }
            else if(walls[wall.index + 1].placed || walls[wall.index - 1].placed){//no need to check if it is a board edge

                return true;
            }
        }
        if(wall.index > 63 && wall.index < 72){
            if(walls[wall.index + 8].placed){
                return true;
            }
        }
        if(wall.index > 71 && wall.index < 120){
            if(walls[wall.index + 8].placed || walls[wall.index - 8].placed){
                return true;
            }
        }
        if(wall.index > 119){
            if(walls[wall.index - 8].placed){
                return true;
            }
        }
        return false;
    }
    
    bool isPerpendicularWall(Wall wall){//verifies if there is already a perpendicular wall on said position
        if(wall.index < 64){
            if(walls[wall.index + 64].placed){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            if(walls[wall.index - 64].placed){
                return true;
            }
            else{
                return false;
            }
        }
    }
    
    bool canPutWall(Wall &wall){//verifies if it is possible to put a wall on said position
        //if there is a wall, part of a wall or a perpendicular wall, it is not possible to put a wall
        if(isWall(wall) || isPartWall(wall) || isPerpendicularWall(wall)){
            return false;
        }
        if(!canBeWall(wall)){
            return false;
        }
        bool player1Can = false;
        bool player2Can = false;
        int player1ObjectiveNodes[] = {0,1,2,3,4,5,6,7,8};
        int player2ObjectiveNodes[] = {72,73,74,75,76,77,78,79,80};
        putWall(wall);
        for(int i = 0; i < 9; i++){
            if(isPath(board[player1Pos], board[player1ObjectiveNodes[i]]) && !player1Can){
                player1Can = true;
            }
            if(isPath(board[player2Pos], board[player2ObjectiveNodes[i]]) && !player2Can){
                player2Can = true;
            }
            if(player1Can && player2Can){
                break;
            }
        }
        removeWall(wall);
        return player1Can && player2Can;     
    }
    
    void putWall(Wall &wall){//puts a wall on said position order of picking nodes must be the same in between sides
        if(board[wall.nodeSide11].up == &board[wall.nodeSide21]){
            board[wall.nodeSide11].up = nullptr;
            board[wall.nodeSide21].down = nullptr;
        }
        else if(board[wall.nodeSide11].down == &board[wall.nodeSide21]){
            board[wall.nodeSide11].down = nullptr;
            board[wall.nodeSide21].up = nullptr;
        }
        else if(board[wall.nodeSide11].left == &board[wall.nodeSide21]){
            board[wall.nodeSide11].left = nullptr;
            board[wall.nodeSide21].right = nullptr;
        }
        else if(board[wall.nodeSide11].right == &board[wall.nodeSide21]){
            board[wall.nodeSide11].right = nullptr;
            board[wall.nodeSide21].left = nullptr;
        }

        if(board[wall.nodeSide12].up == &board[wall.nodeSide22]){
            board[wall.nodeSide12].up = nullptr;
            board[wall.nodeSide22].down = nullptr;
        }
        else if(board[wall.nodeSide12].down == &board[wall.nodeSide22]){
            board[wall.nodeSide12].down = nullptr;
            board[wall.nodeSide22].up = nullptr;
        }
        else if(board[wall.nodeSide12].left == &board[wall.nodeSide22]){
            board[wall.nodeSide12].left = nullptr;
            board[wall.nodeSide22].right = nullptr;
        }
        else if(board[wall.nodeSide12].right == &board[wall.nodeSide22]){
            board[wall.nodeSide12].right = nullptr;
            board[wall.nodeSide22].left = nullptr;
        }
        wall.placed = true;
    }
    
    void removeWall(Wall &wall){
        if(wall.nodeSide11 + 9 == wall.nodeSide21){
            board[wall.nodeSide11].down = &board[wall.nodeSide21];
            board[wall.nodeSide21].up = &board[wall.nodeSide11];
        }
        else if(wall.nodeSide11 - 9 == wall.nodeSide21){
            board[wall.nodeSide11].up = &board[wall.nodeSide21];
            board[wall.nodeSide21].down = &board[wall.nodeSide11];
        }
        else if(wall.nodeSide11 + 1 == wall.nodeSide21){
            board[wall.nodeSide11].right = &board[wall.nodeSide21];
            board[wall.nodeSide21].left = &board[wall.nodeSide11];
        }
        else if(wall.nodeSide11 - 1 == wall.nodeSide21){
            board[wall.nodeSide11].left = &board[wall.nodeSide21];
            board[wall.nodeSide21].right = &board[wall.nodeSide11];
        }

        if(wall.nodeSide12 + 9 == wall.nodeSide22){
            board[wall.nodeSide12].down = &board[wall.nodeSide22];
            board[wall.nodeSide22].up = &board[wall.nodeSide12];
        }
        else if(wall.nodeSide12 - 9 == wall.nodeSide22){
            board[wall.nodeSide12].up = &board[wall.nodeSide22];
            board[wall.nodeSide22].down = &board[wall.nodeSide12];
        }
        else if(wall.nodeSide12 + 1 == wall.nodeSide22){
            board[wall.nodeSide12].right = &board[wall.nodeSide22];
            board[wall.nodeSide22].left = &board[wall.nodeSide12];
        }
        else if(wall.nodeSide12 - 1 == wall.nodeSide22){
            board[wall.nodeSide12].left = &board[wall.nodeSide22];
            board[wall.nodeSide22].right = &board[wall.nodeSide12];
        }
        wall.placed = false;
    }
    
    void move(Node& from, Node& to){ //moves a player from one node to another PS: this function has no regard for the rules of the game
        if(from.player == 1){
            player1Pos = &to - board;
        }
        else{
            player2Pos = &to - board;
        }
        to.player = from.player;
        from.player = 0;
    }
    
    void player1BestMove(){
        
    }
    
    void printBoard(){
        for(int i = 0; i < 81; i++){
            if(i % 9 == 0){
                cout << endl;
            }
            cout << board[i].player << " ";
            
        }
        cout << endl;
    }

    int eval() {
        int player1ObjectiveNodes[] = {0,1,2,3,4,5,6,7,8};
        int player2ObjectiveNodes[] = {72,73,74,75,76,77,78,79,80};
        int shortestPathPlayer1 = 1000000;  // Initialize to "infinity"
        int shortestPathPlayer2 = 1000000;  // Initialize to "infinity"

        for(int i = 0; i < 9; i++) {
            if(isPath(board[player1Pos], board[player1ObjectiveNodes[i]])) {
                int pathLength = shortestPath(board[player1Pos], board[player1ObjectiveNodes[i]]);
                if(pathLength < shortestPathPlayer1){
                    shortestPathPlayer1 = pathLength;
                }
            }
            if(isPath(board[player2Pos], board[player2ObjectiveNodes[i]])) {
                int pathLength = shortestPath(board[player2Pos], board[player2ObjectiveNodes[i]]);
                if(pathLength < shortestPathPlayer2){
                    shortestPathPlayer2 = pathLength;
                }
            }
        }

        return shortestPathPlayer2 - shortestPathPlayer1;
    }
};
//--------------------------------------------
class GraphicalGame {
    public:
        GraphicalGame();
        void run();
        void draw();
        void processInput();

    private:
        sf::RenderWindow window;
        sf::RectangleShape squareShape;
        sf::CircleShape player1Shape;
        sf::CircleShape player2Shape;
        sf::RectangleShape horizontalWallShape;
        sf::RectangleShape verticalWallShape;
        Game game;

        float gap;
        void drawBoard();
        void drawSquare(float x, float y, int player);
        void drawPlayer(float x, float y, int player);
        void drawVerticalWall(float x, float y);
        void drawHorizontalWall(float x, float y);
        void drawWalls();
    };

    GraphicalGame::GraphicalGame() : window(sf::VideoMode(620, 620), "Quoridor Game") {
        gap = 10;
        squareShape.setSize(sf::Vector2f(60, 60));
        squareShape.setFillColor(sf::Color(150, 150, 150));//gray

        player1Shape.setRadius(20);
        player1Shape.setFillColor(sf::Color::Blue);

        player2Shape.setRadius(20);
        player2Shape.setFillColor(sf::Color::Red);

        horizontalWallShape.setSize(sf::Vector2f(130, 10));
        horizontalWallShape.setFillColor(sf::Color(0,150,0)); //green
        
        verticalWallShape.setSize(sf::Vector2f(10, 130));
        verticalWallShape.setFillColor(sf::Color(0,150,0));
    }

    void GraphicalGame::run() {
        int i = 0;
        while (window.isOpen()) {
            processInput();
            draw();
            //sf::sleep(sf::seconds(1));
            //game.move(game.board[game.player1Pos], game.board[game.player1Pos - 9]);

            cout << "eval: " << game.eval() << endl;
        }
    }

    void GraphicalGame::draw() {
        window.clear();
        // Draw the game board
        drawBoard();

        // Draw players
        drawPlayer(game.player1Pos % 9 * (60.0f + gap) + 10, game.player1Pos / 9 * (60.0f + gap) + 10, 1);
        drawPlayer(game.player2Pos % 9 * (60.0f + gap) + 10, game.player2Pos / 9 * (60.0f + gap) + 10, 2);

        // Draw walls
        // Add your code to draw walls here
        drawWalls();
        window.display();
    }

    /* void GraphicalGame::processInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    } */
    void GraphicalGame::processInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } 
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left || event.mouseButton.button == sf::Mouse::Right) {
                    // Get the mouse position
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

                    // Convert mouse position to game board coordinates
                    int col = (mousePosition.x - 10) / (60 + gap);
                    int row = (mousePosition.y - 10) / (60 + gap);

                    // Check if the clicked position is within valid bounds
                    if (col >= 0 && col < 8 && row >= 0 && row < 8) {
                        // Calculate the wall index based on the clicked position
                        int wallIndex = row * 8 + col;

                        // Check if the wall can be placed and place it if possible
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (game.canPutWall(game.walls[wallIndex])) {
                                game.putWall(game.walls[wallIndex]);
                            }
                        } else if (event.mouseButton.button == sf::Mouse::Right) {
                            if (game.canPutWall(game.walls[wallIndex + 64])) {
                                game.putWall(game.walls[wallIndex + 64]);
                            }
                        }
                    }
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                sf::sleep(sf::milliseconds(50));
                // Handle key presses
                if(event.key.code == sf::Keyboard::Up){
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos - 9]);
                }
                else if(event.key.code == sf::Keyboard::Up && sf::Keyboard::Right){
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos - 9]);
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos + 1]);
                }
                else if(event.key.code == sf::Keyboard::Up && sf::Keyboard::Left){
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos - 9]);
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos - 1]);
                }
                else if(event.key.code == sf::Keyboard::Down){
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos + 9]);
                }
                else if(event.key.code == sf::Keyboard::Down && sf::Keyboard::Right){
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos + 9]);
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos + 1]);
                }
                else if(event.key.code == sf::Keyboard::Down && sf::Keyboard::Left){
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos + 9]);
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos - 1]);
                }
                else if(event.key.code == sf::Keyboard::Right){
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos + 1]);
                }
                else if(event.key.code == sf::Keyboard::Right && sf::Keyboard::Up){
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos + 1]);
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos - 9]);
                }
                else if(event.key.code == sf::Keyboard::Right && sf::Keyboard::Down){
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos + 1]);
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos + 9]);
                }
                else if(event.key.code == sf::Keyboard::Left){
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos - 1]);
                }
                else if(event.key.code == sf::Keyboard::Left && sf::Keyboard::Up){
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos - 1]);
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos - 9]);
                }
                else if(event.key.code == sf::Keyboard::Left && sf::Keyboard::Down){
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos - 1]);
                    game.move(game.board[game.player2Pos], game.board[game.player2Pos + 9]);
                }
                
            }
        }
    }




    void GraphicalGame::drawBoard(){
        float initialX = 0;
        float initialY = 0;
        // Draw the game board
        for (int i = 0; i < 81; i++) {
            float x = initialX + (i % 9) * (60.0f + gap);
            float y = initialY + (i / 9) * (60.0f + gap);
            drawSquare(x, y, i);
        }
    }

    void GraphicalGame::drawSquare(float x, float y, int player) {
        squareShape.setPosition(x, y);
        window.draw(squareShape);
        /*if (player == 1) {
            window.draw(squareShape);
            window.draw(player1Shape);
        } else if (player == 2) {
            squareShape.setOutlineColor(sf::Color::Green);
            squareShape.setOutlineThickness(2);
            window.draw(squareShape);
        }*/
    }

    void GraphicalGame::drawPlayer(float x, float y, int player) {
        if (player == 1) {
            player1Shape.setPosition(x, y);
            window.draw(player1Shape);
        } else if (player == 2) {
            player2Shape.setPosition(x, y);
            window.draw(player2Shape);
        }
    }

    void GraphicalGame::drawVerticalWall(float x, float y) {
        verticalWallShape.setPosition(x, y);
        window.draw(verticalWallShape);
    }

    void GraphicalGame::drawHorizontalWall(float x, float y) {
        horizontalWallShape.setPosition(x, y);
        window.draw(horizontalWallShape);

    }

    void GraphicalGame::drawWalls() {

        for(int i = 0; i < 128; i++){
            if(i < 64){
                float x = i % 8 * (60.0f + gap);
                float y = i / 8 * (60.0f + gap) + 60;
                if (game.isWall(game.walls[i])) {
                    drawHorizontalWall(x, y);
                }
            }
            else{
                float x = (i - 64) % 8 * (60.0f + gap);
                float y = (i - 64) / 8 * (60.0f + gap);
                if (game.isWall(game.walls[i])) {
                    drawVerticalWall(x + 60.0f, y);
                }
            }
        }
        
    }
    //--------------------------------------------
int main() {
    GraphicalGame game;
    game.run();
    return 0;
}

/* Determine whether arguments can be added without overflow */
int tadd_ok(int x, int y) {
    int sum = x + y;
    int negOver = x < 0 && y < 0 && sum >= 0;
    int posOver = x >= 0 && y >= 0 && sum < 0;
    
    return !negOver && !posOver;
}

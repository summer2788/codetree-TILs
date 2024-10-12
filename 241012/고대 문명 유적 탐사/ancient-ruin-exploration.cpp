#include <iostream>
#include <tuple>
#include <array>
#include <vector>
#include <algorithm>
#define MaxN 5

using namespace std;
using fourT = tuple<int, int, int, int>; //-value, angle, c, r

vector<vector<int>> ruin(MaxN + 1, vector<int>(MaxN + 1, 0));
vector<vector<int>> visited(MaxN + 1, vector<int>(MaxN + 1, 0));
vector<vector<int>> visited2(MaxN + 1, vector<int>(MaxN + 1, 0));
vector<int> wall;

constexpr std::array<pair<int, int>, 9> arr90{{{-1, 1}, {0, 1}, {1, 1}, {-1, 0}, {0, 0}, {1, 0}, {-1, -1}, {0, -1}, {1, -1}}};
constexpr std::array<pair<int, int>, 9> arr180{{{1, 1}, {1, 0}, {1, -1}, {0, 1}, {0, 0}, {0, -1}, {-1, 1}, {-1, 0}, {-1, -1}}};
constexpr std::array<pair<int, int>, 9> arr270{{{1, -1}, {0, -1}, {-1, -1}, {1, 0}, {0, 0}, {-1, 0}, {1, 1}, {0, 1}, {-1, 1}}};

int dx[] = {-1, 0, 1, 0}; // up, right, down, left
int dy[] = {0, 1, 0, -1}; 
int wallIdx = 0;

void resetVisited()
{
    // reset the visited array to 0
    for (auto &row : visited)
    {
        fill(row.begin(), row.end(), 0);
    }
}

void resetVisited2()
{
    // reset the visited array to 0
    for (auto &row : visited2)
    {
        fill(row.begin(), row.end(), 0);
    }
}

void rotateAngle(vector<vector<int>> &newRuin, int centerR, int centerC, int angle, vector<vector<int>>& oldRuin)
{
    int idx = 0;
    for (int r = centerR - 1; r <= centerR + 1; r++)
    {
        for (int c = centerC - 1; c <= centerC + 1; c++)
        {
            int x;
            int y;
            if (angle == 0)
            { // 90 degree
                tie(x, y) = arr90[idx++];
            }
            else if (angle == 1)
            {
                tie(x, y) = arr180[idx++];
            }
            else
            {
                tie(x, y) = arr270[idx++];
            }
            newRuin[centerR + x][centerC + y] = oldRuin[r][c];
        }
    }
}

void dfs(int r, int c, vector<vector<int>> &ruinCopy, int &maxSeen, int piece)
{
    if (r < 1 || r > MaxN || c < 1 || c > MaxN || (visited[r][c] == true) || (ruinCopy[r][c] != piece))
    {

		return;
    }

    visited[r][c] = true;
    maxSeen++;

    for (int i = 0; i < 4; i++)
    {
        dfs(r + dx[i], c + dy[i], ruinCopy, maxSeen , piece);
    }
}

void dfs2(int r, int c, vector<vector<int>> &ruin, int piece) // for erasing the jewerly
{
    if (r < 1 || r > MaxN || c < 1 || c > MaxN || visited2[r][c] == true || ruin[r][c] != piece)
    {
        return;
    }

    visited2[r][c] = true;
    ruin[r][c] = -1;

    for (int i = 0; i < 4; i++)
    {
        dfs2(r + dx[i], c + dy[i], ruin, piece);
    }
}

fourT explore(vector<vector<int>>& mainRuin)
{
    vector<fourT> temp;
    for (int r = 2; r <= 4; r++) // loop each center
    {
        for (int c = 2; c <= 4; c++) // loop each center
        {
            vector<vector<int>> ruinCopy{mainRuin};

            // rotate the matrix
            for (int angle = 0; angle < 3; angle++)
            { // 0 = 90 , 1 = 180 , 2 = 270
                rotateAngle(ruinCopy, r, c, angle, mainRuin);

                int value = 0;
                for (int i = 1; i < MaxN + 1; i++)
                {
                    for (int j = 1; j < MaxN + 1; j++)
                    {
                        int maxSeen = 0;
                        int piece = ruinCopy[i][j];
                        dfs(i, j, ruinCopy, maxSeen,  piece);
                        if (maxSeen >= 3)
                        {
                            value += maxSeen;
                        }
                    }
                }

                // reset the visited array to 0
                resetVisited();

                // push (-value, angle, c,r) to the vector temp
                temp.push_back({-value, angle, c, r});
            }
        }
    }
    sort(temp.begin(), temp.end());
    return temp[0];
}

//  fill from the wall function
// return the next wall start idx
int fillRuin(int start, vector<vector<int>>& ruin)
{
    int startIdx = start;
    for (int j = 1; j <= MaxN; j++)
    {
        for (int i = MaxN; i >= 1; i--)
        {
            if (ruin[i][j] == -1)
            {
                ruin[i][j] = wall[startIdx++];
            }
        }
    }

    return startIdx;
}

int eraseJewerly(vector<vector<int>>& ruin)
{
    int sum = 0;
    for (int i = 1; i < MaxN + 1; i++)
    {
        for (int j = 1; j < MaxN + 1; j++)
        {
            int maxSeen = 0;
            int piece = ruin[i][j];
            dfs(i, j, ruin, maxSeen,  piece);

            if (maxSeen >= 3)
            { // rewrite the jewerly piece as -1
                sum += maxSeen;
                dfs2(i, j, ruin, piece);
            }
        }
    }

    resetVisited();
    resetVisited2();

    return sum;
}

int successive(vector<vector<int>>& ruin)
{
    int result = 0;

    // first erase the jewerly from the first exploration
    eraseJewerly(ruin);


    // fill the ruin from the wall
    wallIdx = fillRuin(wallIdx, ruin);

    // again search the jewerly , if so delete and fill again
    while (true)
    {
        int sum = eraseJewerly(ruin);
        wallIdx = fillRuin(wallIdx, ruin);

        if (sum == 0)
            break;
        else
        {
            result += sum;
        }
    }

    return result;
}

int main()
{
    int K, M;
    cin >> K >> M;

    int num;
    for (int i = 1; i < MaxN + 1; i++)
    {
        for (int j = 1; j < MaxN + 1; j++)
        {
            cin >> num;
            ruin[i][j] = num;
        }
    }

    wall.resize(M);
    for (int i = 0; i < M; i++)
    {
        cin >> num;
        wall[i] = num;
    }


    for (int i = 0; i < K; i++)
    {
		vector<vector<int>> mainRuin{ ruin };
        int result = 0;

        int value, angle, c, r;
        tie(value, angle, c, r) = explore(ruin);
        value = -value;
        if (value == 0)
        {
            return 0;
        }

        result += value;
        rotateAngle(ruin, r, c, angle, mainRuin);
        result += successive(ruin);

        std::cout << result << ' ';
    }

    return 0;
}
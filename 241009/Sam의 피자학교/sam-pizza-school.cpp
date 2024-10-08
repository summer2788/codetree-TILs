#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;
vector<int> arr;

int Kfind(int n, vector<int> &arr)
{
    vector<vector<int>> dow;
    dow.resize(n, vector<int>(n, 0));

    // step 1 add the powder
    int minV = *std::min_element(arr.begin(), arr.end());
    for (int i = 0; i < n; i++)
    {
        if (arr[i] == minV)
        {
            arr[i]++;
        }

        dow[n - 1][i] = arr[i];
    }

    // step 2 rolling dow

    int line = 1;
    int start = 0;
    int oldLength = 0;
    int oldWidth = 0;

    int area = n - line;

    int newLength;
    int newWidth;

    while (true)
    {
        newLength = line - start;
        newWidth = 1 + oldLength;
        area = n - line;
        if (newLength * newWidth > area)
        {
            break;
        }

        // rotate
        vector<int> temp;

        for (int i = start; i < line; i++)
        {
            for (int j = n - 1; j > (n - 1 - newWidth); j--)
            {
                temp.push_back(dow[j][i]);
            }
        }

        oldWidth = newLength;
        oldLength = newLength;

        int tempLength = newLength;
        newLength = newWidth;
        newWidth = tempLength;

        start = line;
        line += newLength;

        // stack the rotate matrix on the dow
        int k = 0;
        for (int i = n - 1 - newWidth; i < n - 1; i++)
        {
            for (int j = start; j < line; j++)
            {
                dow[i][j] = temp[k++];
            }
        }
    }

    // step 3. press the dow
    // first left to right in layer dow

    // make the new Dow
    vector<vector<int>> dow2(n, vector<int>(n, 0));

    for (int i = (n - newWidth); i <= (n - 1); i++)
    {
        for (int j = start; j < (line - 1); j++)
        {
            int a = dow[i][j];
            int b = dow[i][j + 1];
            int d = std::abs(a - b) / 5;
            if (a > b)
            {
                dow2[i][j] = (dow[i][j] - d);
                dow2[i][j + 1] = (dow[i][j + 1] + d);
            }
            else
            {
                dow2[i][j] = (dow[i][j] + d);
                dow2[i][j + 1] = (dow[i][j + 1] - d);
            }
        }
    }

    // left to right unlayered dow
    for (int i = line; i <= (n - 2); i++)
    {
        int a = dow[n - 1][i];
        int b = dow[n - 1][i + 1];
        int d = std::abs(a - b) / 5;
        if (a > b)
        {
            dow2[n - 1][i] = (dow[n - 1][i] - d);
            dow2[n - 1][i + 1] = (dow[n - 1][i + 1] + d);
        }
        else
        {
            dow2[n - 1][i] = (dow[n - 1][i] + d);
            dow2[n - 1][i + 1] = (dow[n - 1][i + 1] - d);
        }
    }
    // border process
    if (line <= (n - 1) && line > 0)
    {
        int a = dow[n - 1][line - 1];
        int b = dow[n - 1][line];
        int d = std::abs(a - b) / 5;
        if (a > b)
        {
            dow2[n - 1][line - 1] -= d;
            dow2[n - 1][line] += d;
        }
        else
        {
            dow2[n - 1][line - 1] += d;
            dow2[n - 1][line] -= d;
        }
    }

    // up to down layered dow

    for (int i = start; i < line; i++)
    {
        for (int j = n - 1; j > (n - newWidth); j--)
        {
            int a = dow[j][i];
            int b = dow[j - 1][i];
            int d = std::abs(a - b) / 5;

            if (a > b)
            {
                dow2[j][i] -= d;
                dow2[j - 1][i] += d;
            }
            else
            {
                dow2[j][i] += d;
                dow2[j - 1][i] -= d;
            }
        }
    }

    // flatten the vector
    vector<int> temp2;
    // flatten layered dow
    for (int i = start; i < line; i++)
    {
        for (int j = n - 1; j > (n - 1 - newWidth); j--)
        {
            temp2.push_back(dow2[j][i]);
        }
    }

    // flatten the unlayered one
    for (int i = line; i <= (n - 1); i++)
    {
        temp2.push_back(dow2[n - 1][i]);
    }

    // step 4. fold twice

    line = n / 2;
    // rewrite in dow2
    // put the base dow first
    for (int i = line; i < n; i++)
    {
        dow2[n - 1][i] = temp2[i];
    }

    // fold first
    vector<int> temp3;
    for (int i = line - 1; i >= 0; i--)
    {
        temp3.push_back(temp2[i]);
    }

    // put second layer to the top
    int idx = 0;
    for (int i = line; i < n; i++)
    {
        dow2[n - 2][i] = temp3[idx++];
    }

    // second fold

    start = line;
    line = (start + n) / 2;

    newWidth = 2;
    newLength = line - start;

    vector<int> temp4;
    // flatten and push into the temp4
    for (int i = n - 1; i >= n - 2; i--)
    {
        for (int j = line - 1; j >= start; j--)
        {
            temp4.push_back(dow2[i][j]);
        }
    }

    // push in the the top
    idx = 0;
    for (int i = n - 4; i <= n - 3; i++)
    {
        for (int j = line; j < n; j++)
        {
            dow2[i][j] = temp4[idx++];
        }
    }

    // last
    // repeat step 3
    // first left to right
    for (int i = n - 4; i < n; i++)
    {
        for (int j = line; j < n - 1; j++)
        {
            int a = dow2[i][j];
            int b = dow2[i][j + 1];
            int d = std::abs(a - b) / 5;

            if (a > b)
            {
                dow[i][j] = a - d;
                dow[i][j + 1] = b + d;
            }
            else
            {
                dow[i][j] = a + d;
                dow[i][j + 1] = b - d;
            }
        }
    }

    // down to up
    for (int i = line; i < n; i++)
    {
        for (int j = n - 1; j >= n - 3; j--)
        {
            int a = dow2[j][i];
            int b = dow2[j - 1][i];
            int d = std::abs(a - b) / 5;

            if (a > b)
            {
                dow[j][i] -= d;
                dow[j - 1][i] += d;
            }
            else
            {
                dow[j][i] += d;
                dow[j - 1][i] -= d;
            }
        }
    }

    // flatten finally
    idx = 0;
    for (int i = line; i < n; i++)
    {
        for (int j = n - 1; j >= n - 4; j--)
        {
            arr[idx++] = dow[j][i];
        }
    }

    // find min element and max element and return diference
    int resMin = arr[0];
    int resMax = arr[0];
    for (int i = 1; i < arr.size(); i++)
    {
        if (arr[i] < resMin)
        {
            resMin = arr[i];
        }

        if (arr[i] > resMax)
        {
            resMax = arr[i];
        }
    }

    return resMax - resMin;
}

int main()
{

    int n;
    int k;
    int num;

    cin >> n >> k;
    arr.resize(n, 0);

    for (int i = 0; i < n; i++)
    {
        cin >> num;
        arr[i] = num;
    }

    int cnt = 1;
    int minV = *std::min_element(arr.begin(), arr.end());
    int maxV = *std::max_element(arr.begin(), arr.end());
    if (maxV - minV <= k)
    {
        cout << 0 << '\n';
        return 0;
    }

    while (Kfind(n, arr) > k)
    {
        cnt++;
    }

    cout << cnt << '\n';

    return 0;
}
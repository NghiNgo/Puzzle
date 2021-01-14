// Chương trình in đường dẫn từ nút gốc đến nút đích
// cho thuật toán câu đố N * N -1 sử dụng Branch and Bound
// Giải pháp giả định rằng trường hợp câu đố có thể giải được
#include <bits/stdc++.h>
using namespace std;
#define N 3

// các nút của cây không gian trạng thái
struct Node
{
    // lưu trữ nút cha của nút hiện tại
    // giúp truy tìm đường dẫn khi câu trả lời được tìm thấy
    Node *parent;

    // lưu trữ ma trận
    int mat[N][N];

    // lưu trữ tọa độ ô trống
    int x, y;

    // lưu trữ số lượng ô xếp không đúng vị trí
    int cost;

    // lưu trữ số lần di chuyển cho đến nay
    int level;
};

// Hàm in ma trận N x N
int printMatrix(int mat[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
}

// Hàm cấp phát một nút mới
Node *newNode(int mat[N][N], int x, int y, int newX,
              int newY, int level, Node *parent)
{
    Node *node = new Node;

    // đặt con trỏ cho đường dẫn đến thư mục gốc
    node->parent = parent;

    // sao chép dữ liệu từ nút cha sang nút hiện tại
    memcpy(node->mat, mat, sizeof node->mat);

    // di chuyển ô theo 1 vị trí
    swap(node->mat[x][y], node->mat[newX][newY]);

    // đặt số ô đặt không đúng vị trí
    node->cost = INT_MAX;

    // đặt số lần di chuyển cho đến nay
    node->level = level;

    // cập nhật tọa độ ô trống mới
    node->x = newX;
    node->y = newY;

    return node;
}

// bottom, left, top, right
int row[] = {1, 0, -1, 0};
int col[] = {0, -1, 0, 1};

// Hàm tính số ô đặt sai vị trí
// I E. số ô không trống không ở vị trí mục tiêu của chúng
int calculateCost(int initial[N][N], int final[N][N])
{
    int count = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (initial[i][j] && initial[i][j] != final[i][j])
                count++;
    return count;
}

// Hàm kiểm tra xem (x, y) có phải là một tọa độ ma trận hợp lệ hay không
int isSafe(int x, int y)
{
    return (x >= 0 && x < N && y >= 0 && y < N);
}

// in đường dẫn từ nút gốc đến nút đích
void printPath(Node *root)
{
    if (root == NULL)
        return;
    printPath(root->parent);
    printMatrix(root->mat);

    printf("\n");
}

// Đối tượng so sánh được sử dụng để sắp xếp đống
struct comp
{
    bool operator()(const Node *lhs, const Node *rhs) const
    {
        return (lhs->cost + lhs->level) > (rhs->cost + rhs->level);
    }
};

// Hàm giải thuật toán câu đố N * N - 1 bằng
// Branch và Bound. x và y là tọa độ ô trống
// ở trạng thái ban đầu
void solve( int initial[N][N], int x, int y,
            int final[N][N])
{
    // Tạo một hàng đợi ưu tiên để lưu trữ các nút trực tiếp của
    // cây tìm kiếm;
    priority_queue<Node *, std::vector<Node *>, comp> pq;

    // tạo một nút gốc và tính toán chi phí của nó
    Node *root = newNode(initial, x, y, x, y, 0, NULL);
    root->cost = calculateCost(initial, final);

    // Thêm root vào danh sách các nút trực tiếp;
    pq.push(root);

    // Tìm một nút trực tiếp với chi phí thấp nhất,
    // thêm các nút con của nó vào danh sách các nút trực tiếp và
    // cuối cùng xóa nó khỏi danh sách.
    while (!pq.empty())
    {
        // Tìm một nút trực tiếp với chi phí ước tính thấp nhất
        Node *min = pq.top();

        // Nút tìm thấy bị xóa khỏi danh sách
        // các nút trực tiếp
        pq.pop();

        // nếu min là một nút trả lời
        if (min->cost == 0)
        {
            // in đường dẫn từ gốc đến đích;
            printPath(min);
            return;
        }

        // làm cho mỗi con của min
        // tối đa 4 con cho một nút
        for (int i = 0; i < 4; i++)
        {
            if (isSafe(min->x + row[i], min->y + col[i]))
            {
                // tạo một nút con và tính toán
                // chi phí của nó
                Node *child = newNode(  min->mat, min->x,
                                        min->y, min->x + row[i],
                                        min->y + col[i],
                                        min->level + 1, min);
                child->cost = calculateCost(child->mat, final);

                // Thêm con vào danh sách các nút trực tiếp
                pq.push(child);
            }
        }
    }
}

// Mã trình điều khiển
int main()
{
    // Cấu hình ban đầu
    // Giá trị 0 được sử dụng cho không gian trống
    int initial[N][N] =
        {
            {1, 8, 2},
            {0, 4, 3},
            {7, 6, 5}};

    // Cấu hình cuối cùng có thể giải quyết
    // Giá trị 0 được sử dụng cho không gian trống
    int final[N][N] =
        {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 0}};

    // Tọa độ ô trống trong ban đầu
    // cấu hình
    int x = 1, y = 0;

    solve(initial, x, y, final);
    system("pause");
    return 0;
}
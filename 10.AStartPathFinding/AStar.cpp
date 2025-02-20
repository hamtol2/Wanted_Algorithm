#include "AStar.h"
#include "Node.h"


AStar::AStar()
{
}

AStar::~AStar()
{
    // �޸� ����.
    for (Node* node : openList)
    {
        SafeDelete(node);
    }

    openList.clear();

    for (Node* node : closedList)
    {
        SafeDelete(node);
    }

    closedList.clear();
}

std::vector<Node*> AStar::FindPath(Node* startNode, Node* goalNode, const std::vector<std::vector<int>>& grid)
{
    // ���� ����/��ǥ ���� ����.
    this->startNode = startNode;
    this->goalNode = goalNode;

    // ���� ��ġ�� ���� ����Ʈ�� �߰�.
    openList.emplace_back(startNode);

    // �����¿� �� �밢�� �̵� ���� �� ��� ���.
    std::vector<Direction> directions =
    {
        // �ϻ���� �̵�.
        { 0, 1, 1.0f }, { 0, -1, 1.0f }, { 1, 0, 1.0f }, { -1, 0, 1.0f },

        // �밢�� �̵�.
        { 1, 1, 1.414f }, { 1, -1, 1.414f }, { -1, 1, 1.414f }, { -1, -1, 1.414f }
    };

    // �̿� ��� Ž��(���� ����Ʈ�� ��� ���� ������ �ݺ�).
    while (!openList.empty())
    {
        // ���� ���� ����Ʈ���� ���(fCost)�� ���� ���� ��� �˻�.
        // ����ȭ ����(�켱 ���� ť, ��).
        Node* lowestNode = openList[0];
        for (Node* node : openList)
        {
            if (node->fCost < lowestNode->fCost)
            {
                lowestNode = node;
            }
        }

        // ����� ���� ���� ��带 ���� ���� ����.
        Node* currentNode = lowestNode;

        // ���� ��尡 ��ǥ ������� Ȯ��.
        if (IsDestination(currentNode))
        {
            // ��ǥ �������κ��� �������ؼ� ��θ� ���� �� ��ȯ.
            return ConstructPath(currentNode);
        }

        // �湮 ó���� ���� ���� ��带 ���� ����Ʈ���� ����.
        for (int ix = 0; ix < (int)openList.size(); ++ix)
        {
            // �� ����� ��ġ�� ������� ������ Ȯ��.
            if (*openList[ix] == *currentNode)
            {
                openList.erase(openList.begin() + ix);
                break;
            }
        }

        // �湮 ó���� ���� ���� ����Ʈ�� �߰�.
        // �̹� �߰��� ���� ����.
        //bool isNodeInList = false;
        //for (Node* node : closedList)
        //{
        //    // ��ġ ������� ��.
        //    if (*node == *currentNode)
        //    {
        //        isNodeInList = true;
        //        break;
        //    }
        //}

        //// �̹� ������ ����.
        //if (isNodeInList)
        //{
        //    continue;
        //}

        // ������ ���� ����Ʈ�� �߰�.
        closedList.emplace_back(currentNode);

        // �̿���� �湮 (�ϻ���� .. ���ʷ� �湮).
        for (const Direction& direction : directions)
        {
            // ������ �̵��� ��ġ Ȯ��.
            int newX = currentNode->position.x + direction.x;
            int newY = currentNode->position.y + direction.y;

            // �׸��� ����� ����.
            if (!IsInRange(newX, newY, grid))
            {
                continue;
            }

            // �̵��� ��尡 �̵� �������� Ȯ��.
            if (grid[newY][newX] == 1)
            {
                continue;
            }

            // �̹� �湮�ߴ��� Ȯ��.
            if (HasVisited(newX, newY, currentNode->gCost + direction.cost))
            {
                continue;
            }

            // �湮�� ��� ����(��� ���).
            Node* neighborNode = new Node(newX, newY, currentNode);
            neighborNode->gCost = currentNode->gCost + direction.cost;
            neighborNode->hCost = CalculateHeuristic(neighborNode, goalNode);
            neighborNode->fCost = neighborNode->gCost + neighborNode->hCost;

            // ���� ����Ʈ�� �߰�.
            // �̹� ����Ʈ�� �ִ��� Ȯ��.
            Node* openListNode = nullptr;
            for (Node* node : openList)
            {
                if (*node == *neighborNode)
                {
                    openListNode = node;
                    break;
                }
            }

            // �̿� ��尡 ����Ʈ�� ������ �ٷ� �߰�.
            // ����Ʈ�� ������ ����� ���� �� �����ϸ� �߰�.
            if (openListNode == nullptr
                || neighborNode->gCost < openListNode->gCost
                || neighborNode->fCost < openListNode->fCost)
            {
                openList.emplace_back(neighborNode);
            }
            else
            {
                // ����Ʈ �߰� ����� �ƴ϶��, �޸� ����.
                SafeDelete(neighborNode);
            }
        }
    }

    // Ž�� ����.
    // initialize_list (c++).
    //return {};
    return std::vector<Node*>();
}

void AStar::DisplayGridWithPath(
    std::vector<std::vector<int>>& grid, const std::vector<Node*>& path)
{
    // ��ο� �ִ� ��ġ�� 2�� ǥ��.
    for (const Node* node : path)
    {
        grid[node->position.y][node->position.x] = 2;
    }

    // �� ���.
    for (int y = 0; y < (int)grid.size(); ++y)
    {
        for (int x = 0; x < (int)grid[0].size(); ++x)
        {
            // ��ֹ�.
            if (grid[y][x] == 1)
            {
                std::cout << "1 ";
            }

            // ���.
            else if (grid[y][x] == 2)
            {
                std::cout << "+ ";
            }

            // ��.
            else if (grid[y][x] == 0)
            {
                std::cout << "0 ";
            }
        }

        std::cout << "\n";
    }
}

std::vector<Node*> AStar::ConstructPath(Node* goalNode)
{
    // ��ǥ ���κ��� �θ� ��带 ���� �������ϸ鼭 ��� ����.
    std::vector<Node*> path;
    this->goalNode = goalNode;
    Node* currentNode = goalNode;
    while (currentNode != nullptr)
    {
        path.emplace_back(currentNode);
        currentNode = currentNode->parent;
    }

    // ���� �������� ��ǥ �������� ���ϵ��� �迭 ������.
    std::reverse(path.begin(), path.end());
    return path;
}

float AStar::CalculateHeuristic(Node* currentNode, Node* goalNode)
{
    // ���� ��忡�� ��ǥ �������� ���� ���.
    Position diff = *currentNode - *goalNode;
    return std::sqrtf(diff.x * diff.x + diff.y * diff.y);
}

bool AStar::IsInRange(int x, int y, const std::vector<std::vector<int>>& grid)
{
    // ��� ������� Ȯ��.
    if (x < 0 || x >= grid[0].size() || y < 0 || y >= grid.size())
    {
        return false;
    }

    return true;
}

bool AStar::HasVisited(int x, int y, float gCost)
{
    for (int ix = 0; ix < (int)closedList.size(); ++ix)
    {
        Node* node = closedList[ix];
        if (node->position.x == x && node->position.y == y)
        {
            // ��ġ�� ����, ����� ��ũ�� �湮�� ���� ����.
            return true;
        }
    }

    // ����/���� ����Ʈ�� �̹� �ش� ��ġ�� ������ �湮�� ������ �Ǵ�.
    for (int ix = 0; ix < (int)openList.size(); ++ix)
    {
        Node* node = openList[ix];
        if (node->position.x == x && node->position.y == y)
        {
            // ��ġ�� ����, ����� ��ũ�� �湮�� ���� ����.
            if (gCost >= node->gCost)
            {
                return true;
            }

            // �� ����� ����� ���� ��쿡�� ���� ��� ����.
            openList.erase(openList.begin() + ix);
            SafeDelete(node);
        }
    }

    // ����Ʈ�� ������ �湮���� ���� ������ �Ǵ�.
    return false;
}

bool AStar::IsDestination(const Node* node)
{
    return *node == *goalNode;
}
#include "Node.h"
#include "QuadTree.h"

Node::Node(const Bounds& bounds, int depth)
	: bounds(bounds), depth(depth)
{
}

Node::~Node()
{
	// ���� ��忡 ����� ��� ��� ����.
	//for (Node* node : pofloats)
	//{
	//	SafeDelete(node);
	//}

	//pofloats.clear();

	//// �ڽ� ��� ����.
	//ClearChildren();
	Clear();
}

void Node::Insert(Node* node)
{
	// ��ġ�� ���� Ȯ��.
	NodeIndex result = TestRegion(node->GetBounds());

	// ��ġ�� ���� ��忡 �߰�.
	if (result == NodeIndex::Straddling)
	{
		points.emplace_back(node);
	}
	// ��ġ�� ���� ���.
	else if (result != NodeIndex::OutOfArea)
	{
		if (Subdivide())
		{
			if (result == NodeIndex::TopLeft)
			{
				topLeft->Insert(node);
			}
			else if (result == NodeIndex::TopRight)
			{
				topRight->Insert(node);
			}
			else if (result == NodeIndex::BottomLeft)
			{
				bottomLeft->Insert(node);
			}
			else if (result == NodeIndex::BottomRight)
			{
				bottomRight->Insert(node);
			}
		}
		// �� �̻� ���� �� ���� ���.
		else
		{
			points.emplace_back(node);
		}
	}
}

void Node::Query(const Bounds& queryBounds, std::vector<Node*>& possibleNodes)
{
	// ���� ��� �߰�.
	possibleNodes.emplace_back(this);

	// ������ ���ҵ�����, �ڽ� ��嵵 �߰� �˻�.
	if (IsDivided())
	{
		// ���� ���� ������ ��ġ�� 4�и� ��� Ȯ��.
		std::vector<NodeIndex> quads = GetQuads(queryBounds);

		// ��ġ�� 4�и��� �������� �˻� ����(���).
		for (NodeIndex& index : quads)
		{
			if (index == NodeIndex::TopLeft)
			{
				topLeft->Query(queryBounds, possibleNodes);
			}
			else if (index == NodeIndex::TopRight)
			{
				topRight->Query(queryBounds, possibleNodes);
			}
			else if (index == NodeIndex::BottomLeft)
			{
				bottomLeft->Query(queryBounds, possibleNodes);
			}
			else if (index == NodeIndex::BottomRight)
			{
				bottomRight->Query(queryBounds, possibleNodes);
			}
		}
	}
}

void Node::Clear()
{
	// ���� ��� ����.
	for (Node* node : points)
	{
		SafeDelete(node);
	}

	// ����Ʈ ����.
	points.clear();

	// ���ҵ� ��� �ڽ� ����.
	if (IsDivided())
	{
		topLeft->Clear();
		topRight->Clear();
		bottomLeft->Clear();
		bottomRight->Clear();

		ClearChildren();
	}
}

bool Node::Subdivide()
{
	// �ִ� ���̿� ���������� ���̻� �ȳ���.
	if (depth == QuadTree::maxDepth)
	{
		return false;
	}

	// ���� ���� �ȵ�����, ���� ����.
	if (!IsDivided())
	{
		// ������ ������ ���� �� ���.
		float x = bounds.X();
		float y = bounds.Y();
		float halfWidth = bounds.Width() / 2;
		float halfHeight = bounds.Height() / 2;

		// �� 4�и��� �ڽ� ��ü ����.
		topLeft = new Node(Bounds(x, y, halfWidth, halfHeight), depth + 1);
		topRight = new Node(Bounds(x + halfWidth, y, halfWidth, halfHeight), depth + 1);
		bottomLeft = new Node(Bounds(x, y + halfHeight, halfWidth, halfHeight), depth + 1);
		bottomRight = new Node(Bounds(x + halfWidth, y + halfHeight, halfWidth, halfHeight), depth + 1);
	}

	return true;
}

bool Node::IsDivided()
{
	return topLeft != nullptr;
}

void Node::ClearChildren()
{
	// ���� ����ƴ��� Ȯ��.
	if (IsDivided())
	{
		SafeDelete(topLeft);
		SafeDelete(topRight);
		SafeDelete(bottomLeft);
		SafeDelete(bottomRight);
	}
}

NodeIndex Node::TestRegion(const Bounds& bounds)
{
	// ���޵� bounds�� ��ġ�� 4�и� ��� Ȯ��.
	std::vector<NodeIndex> quads = GetQuads(bounds);

	// ��ġ�� ���� ������ ���� ������ ǥ��.
	if (quads.size() == 0)
	{
		return NodeIndex::OutOfArea;
	}

	// �ϳ��� ��ģ ��� �ش� 4�и� ��ȯ.
	if (quads.size() == 1)
	{
		return quads[0];
	}

	// ���� ������ ��ģ ���.
	return NodeIndex::Straddling;
}

std::vector<NodeIndex> Node::GetQuads(const Bounds& bounds)
{
	std::vector<NodeIndex> quads;

	// ���� ��꿡 �ʿ��� ����.
	float x = this->bounds.X();
	float y = this->bounds.Y();
	float halfWidth = this->bounds.Width() / 2.0f;
	float halfHeight = this->bounds.Height() / 2.0f;
	float centerX = x + halfWidth;
	float centerY = y + halfHeight;

	// ���� �����̶� ��ġ���� Ȯ��.
	bool left = bounds.X() < centerX && bounds.MaxX() >= x;

	// ������ �����̶� ��ġ���� Ȯ��.
	bool right = bounds.MaxX() >= centerX && bounds.X() < this->bounds.MaxX();

	// ���� �����̶� ��ġ���� Ȯ��.
	bool top = bounds.Y() < centerY && bounds.MaxY() >= y;

	// �Ʒ��� �����̶� ��ġ���� Ȯ��.
	bool bottom = bounds.MaxY() >= centerY && bounds.Y() < this->bounds.MaxY();

	if (top && left)
	{
		quads.emplace_back(NodeIndex::TopLeft);
	}
	if (top && right)
	{
		quads.emplace_back(NodeIndex::TopRight);
	}
	if (bottom && left)
	{
		quads.emplace_back(NodeIndex::BottomLeft);
	}
	if (bottom && right)
	{
		quads.emplace_back(NodeIndex::BottomRight);
	}

	return quads;
}
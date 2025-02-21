#pragma once

#include "Bounds.h"
#include <vector>

template<typename T>
void SafeDelete(T*& t)
{
	if (t)
	{
		delete t;
		t = nullptr;
	}
}

// ���� ������ ���� ������.
enum class NodeIndex
{
	TopLeft,		// ��輱�� ��ġ�� �ʰ� ���� ����.
	TopRight,
	BottomLeft,
	BottomRight,
	Straddling,		// ��輱�� ��ģ ���.
	OutOfArea		// ������ ��� ���.
};

class Node
{
public:
	Node(const Bounds& bounds, int depth = 0);
	~Node();

	// ����(Insert).
	void Insert(Node* node);

	// ����(Query).
	void Query(const Bounds& queryBounds, std::vector<Node*>& possibleNodes);

	// ����.
	void Clear();

	// Getter.
	// ���� ����� ����.
	Bounds GetBounds() const { return bounds; }

	// ���� ������ ���Ե� ���.
	const std::vector<Node*>& Points() const { return points; }

	// �ڽ� ���.
	Node* TopLeft() const { return topLeft; }
	Node* TopRight() const { return topRight; }
	Node* BottomLeft() const { return bottomLeft; }
	Node* BottomRight() const { return bottomRight; }

private:
	// ������ 4������ �� ����� �Լ�.
	bool Subdivide();

	// ������ ���ҵƴ��� Ȯ���ϴ� �Լ�.
	bool IsDivided();

	// �ڽ� ��� ���� �Լ�.
	void ClearChildren();

	// ������ ������ ��ġ�� ������ ��ȯ�ϴ� �Լ�.
	NodeIndex TestRegion(const Bounds& bounds);

	// ������ ������ ���Եǰų� ��ġ�� ������ ��� ���� �� ����� �Լ�.
	std::vector<NodeIndex> GetQuads(const Bounds& bounds);

private:
	// ���� ����� ����.
	int depth = 0;

	// ���� ����� ����.
	Bounds bounds;

	// ���� ������ ���Ե� ���.
	std::vector<Node*> points;

	// �ڽ� ���.
	Node* topLeft = nullptr;
	Node* topRight = nullptr;
	Node* bottomLeft = nullptr;
	Node* bottomRight = nullptr;
};
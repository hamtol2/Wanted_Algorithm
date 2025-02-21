#include <iostream>
#include "QuadTree.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main()
{
	// �޸� ���� �˻�.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// 0,0�� ���� �������, (200, 200) ũ�⸦ ������ ���� ����.
	QuadTree tree(Bounds(0, 0, 200, 200));

	// ����.
	tree.Insert(new Node(Bounds(60, 80)));
	tree.Insert(new Node(Bounds(50, 50)));
	tree.Insert(new Node(Bounds(90, 120)));
	tree.Insert(new Node(Bounds(150, 150)));
	tree.Insert(new Node(Bounds(10, 10)));

	std::cout << "��� ���� �Ϸ�.\n";

	//Node testNode(Bounds(10, 10));
	Node testNode(Bounds(45, 45, 20, 20));
	std::vector<Node*> intersects = tree.Query(&testNode);
	if (intersects.size() == 0)
	{
		std::cout << "��ġ�� ��带 ã�� ���߽��ϴ�.\n";
	}
	else
	{
		std::cout << "��ġ�� ��带 " << intersects.size() << "�� ã�ҽ��ϴ�.\n";
	}

	std::cin.get();
}
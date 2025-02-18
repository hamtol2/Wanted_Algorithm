#include "RedBlackTree.h"

Node* RedBlackTree::nil = nullptr;

Node* RedBlackTree::CreateNode(int data, Color color)
{
	Node* newNode = new Node(data, color);
	newNode->SetLeft(nil);
	newNode->SetRight(nil);

	return newNode;
}

RedBlackTree::RedBlackTree()
{
	// 초기 설정.
	if (nil == nullptr)
	{
		nil = new Node(0, Color::Black);
	}

	root = nil;
}

RedBlackTree::~RedBlackTree()
{
	DestroyRecursive(root);
	SafeDelete(nil);
}

bool RedBlackTree::Find(int data, Node** outNode)
{
	// 트리가 비었다면, 검색 실패.
	if (root == nil)
	{
		return false;
	}
	
	// 재귀적으로 검색 진행.
	return FindRecursive(root, outNode, data);
}

bool RedBlackTree::Insert(int data)
{
	// 먼저 중복되는 데이터가 있는지 확인.
	Node* node = nullptr;
	if (Find(data, &node))
	{
		std::cout 
			<< "오류 - 노드 추가 실패: 이미 같은 값이 있습니다. 입력 값: " 
			<< data << "\n";

		return false;
	}

	// 노드 생성 후 결과 반환.
	Insert(CreateNode(data, Color::Red));
	return true;
}

void RedBlackTree::Remove(int data)
{
	// 삭제할 노드 검색에 실패하면 삭제 실패.
	Node* deleted = nullptr;
	if (!Find(data, &deleted))
	{
		std::cout 
			<< "오류: 노드 삭제 실패. 삭제할 노드를 찾지 못했습니다. (검색 값: "
			<< data << ")\n";
		return;
	}

	// 삭제 진행.
	RemoveImpl(deleted);
}

void RedBlackTree::Print(int depth, int blackCount)
{
	PrintRecursive(root, depth, blackCount);
}

bool RedBlackTree::FindRecursive(Node* node, Node** outNode, int data)
{
	// 재귀 탈출 조건 (검색 실패).
	if (node == nil)
	{
		return false;
	}

	// 검색에 성공했으면 true 및 outNode 반환.
	if (node->Data() == data)
	{
		*outNode = node;
		return true;
	}

	// 검색할 데이터가 현재 노드 값보다 작으면 왼쪽으로 검색 진행.
	if (node->Data() > data)
	{
		return FindRecursive(node->Left(), outNode, data);
	}
	else
	{
		return FindRecursive(node->Right(), outNode, data);
	}
}

void RedBlackTree::Insert(Node* newNode)
{
	// 트리가 비어 있는 경우, 루트로 설정.
	if (root == nil)
	{
		root = newNode;
		root->SetColor(Color::Black);
		return;
	}

	// 트리가 비어 있지 않으면, 재귀적으로 위치 검색 후 삽입.
	InsertRecursive(root, newNode);

	// 삽입된 노드 값 정리.
	newNode->SetColor(Color::Red);
	newNode->SetLeft(nil);
	newNode->SetRight(nil);

	// 삽입 후 재정렬 처리.
	RestructureAfterInsert(newNode);
}

void RedBlackTree::InsertRecursive(Node* node, Node* newNode)
{
	// 새 노드의 데이터가 현재 노드보다 작으면, 왼쪽 하위 트리로 삽입 진행.
	if (node->Data() > newNode->Data())
	{
		// 왼쪽 자손이 없다면, 삽입 처리.
		if (node->Left() == nil)
		{
			node->SetLeft(newNode);
			newNode->SetParent(node);
			return;
		}

		// 자손이 있으면, 하위 트리로 삽입 과정 진행.
		InsertRecursive(node->Left(), newNode);
	}
	else
	{
		// 오른쪽 자손이 없다면, 삽입.
		if (node->Right() == nil)
		{
			node->SetRight(newNode);
			newNode->SetParent(node);
			return;
		}

		// 자손이 있으면, 하위 트리로 삽입 진행.
		InsertRecursive(node->Right(), newNode);
	}
}

void RedBlackTree::RestructureAfterInsert(Node* newNode)
{
	// 부모 노드가 빨간색인 경우 (Red-Red Conflict).
	while (newNode != root && newNode->Parent()->GetColor() == Color::Red)
	{
		// 삼촌 구하기.
		if (newNode->Parent() == newNode->Parent()->Parent()->Left())
		{
			// 삼촌.
			Node* uncle = newNode->Parent()->Parent()->Right();

			// 삼촌 노드가 빨간색 일 때 (Case1).
			// 해결: 부모와 삼촌을 Black, 할아버지를 Red로 변경 후, 할아버지 기준으로 다시 검사.
			if (uncle->GetColor() == Color::Red)
			{
				newNode->Parent()->SetColor(Color::Black);
				uncle->SetColor(Color::Black);
				newNode->Parent()->Parent()->SetColor(Color::Red);

				newNode = newNode->Parent()->Parent();
				continue;
			}
			
			// 삼촌이 검정색일 때.
			// 해결: 부모를 Black, 할아버지를 Red로 변경 후, 할아버지를 기준으로 회전.
			// Case2/Case3.
			// Case2: 지그재그 형태. 부모의 위치와 나의 위치가 반대
			// (일직선으로 변경 후 Case3처리).
			if (newNode == newNode->Parent()->Right())
			{
				// 부모를 중심으로 회전 처리(일직선으로 만들기 위해).
				newNode = newNode->Parent();
				RotateToLeft(newNode);
			}

			// Case3: 부모와 나의 위치가 일치할 때.
			newNode->Parent()->SetColor(Color::Black);
			newNode->Parent()->Parent()->SetColor(Color::Red);

			RotateToRight(newNode->Parent()->Parent());
		}

		// 부모가 조부모 기준 오른쪽 자손일 때.
		else
		{
			// 삼촌.
			Node* uncle = newNode->Parent()->Parent()->Left();

			// 삼촌 노드가 빨간색 일 때 (Case1).
			// 해결: 부모와 삼촌을 Black, 할아버지를 Red로 변경 후, 할아버지 기준으로 다시 검사.
			if (uncle->GetColor() == Color::Red)
			{
				newNode->Parent()->SetColor(Color::Black);
				uncle->SetColor(Color::Black);
				newNode->Parent()->Parent()->SetColor(Color::Red);

				newNode = newNode->Parent()->Parent();
				continue;
			}

			// 삼촌이 검정색일 때.
			// 해결: 부모를 Black, 할아버지를 Red로 변경 후, 할아버지를 기준으로 회전.
			// Case2/Case3.
			// Case2: 지그재그 형태. 부모의 위치와 나의 위치가 반대
			// (일직선으로 변경 후 Case3처리).
			if (newNode == newNode->Parent()->Left())
			{
				// 부모를 중심으로 회전 처리(일직선으로 만들기 위해).
				newNode = newNode->Parent();
				RotateToRight(newNode);
			}

			// Case3: 부모와 나의 위치가 일치할 때.
			newNode->Parent()->SetColor(Color::Black);
			newNode->Parent()->Parent()->SetColor(Color::Red);

			RotateToLeft(newNode->Parent()->Parent());
		}
	}

	// 루트 노드는 블랙.
	root->SetColor(Color::Black);
}

void RedBlackTree::RestructureAfterRemove(Node* node)
{
	// 더블 블랙 문제 해결.
	while (node->Parent() != nullptr && node->GetColor() == Color::Black)
	{
		// 형제 노드 구하기.
		if (node == node->Parent()->Left())
		{
			Node* sibling = node->Parent()->Right();
			if (sibling == nil)
			{
				break;
			}

			// Case1: 형제 노드가 빨간색.
			// 해결: 형제 노드를 Black으로 변경하고, 
			// 부모를 Red로 바꾼 후 부모 기준으로 좌회전 또는 우회전.
			if (sibling->GetColor() == Color::Red)
			{
				sibling->SetColor(Color::Black);
				node->Parent()->SetColor(Color::Red);

				// 좌회전.
				RotateToLeft(node->Parent());
				continue;
			}

			// Case2: 형제 노드가 검정색일 때.
			// 해결: 형제 노드를 Red로 변경하여 Black Height를 줄이고, 
			// 부모를 새로운 노드로 설정 후 다시 검사.
			if (sibling->Left()->GetColor() == Color::Black
				&& sibling->Right()->GetColor() == Color::Black)
			{
				sibling->SetColor(Color::Red);
				node = node->Parent();
				continue;
			}

			// Case3: 형제 노드 검정. 형제 노드의 왼쪽 자손이 빨간색.
			// 해결: 형제의 왼쪽 자식을 Black으로 변경, 
			// 형제를 Red로 변경 후 형제 기준 우회전.
			if (sibling->Left()->GetColor() == Color::Red)
			{
				sibling->Left()->SetColor(Color::Black);
				sibling->SetColor(Color::Red);

				// 우회전.
				RotateToRight(sibling);

				// 회전 후 형제 위치 업데이트.
				sibling = node->Parent()->Right();
			}

			// Case4: 형제 노드는 Black, 형제의 오른쪽 자식이 Red.
			// 해결: 형제를 부모와 같은 색으로 설정, 
			// 부모를 Black으로 변경, 
			// 형제의 오른쪽 자식을 Black으로 변경 후 부모 기준 좌회전.
			if (sibling->Right()->GetColor() == Color::Red)
			{
				sibling->SetColor(sibling->Parent()->GetColor());
				sibling->Parent()->SetColor(Color::Black);
				sibling->Right()->SetColor(Color::Black);
				RotateToLeft(node->Parent());
			}

			continue;
		}

		// 형제 노드 구하기.
		Node* sibling = node->Parent()->Left();
		if (sibling == nil)
		{
			break;
		}

		// Case1: 형제 노드가 빨간색.
		// 해결: 형제 노드를 Black으로 변경하고, 
		// 부모를 Red로 바꾼 후 부모 기준으로 좌회전 또는 우회전.
		if (sibling->GetColor() == Color::Red)
		{
			sibling->SetColor(Color::Black);
			node->Parent()->SetColor(Color::Red);

			// 좌회전.
			RotateToRight(node->Parent());
			continue;
		}

		// Case2: 형제 노드가 검정색일 때.
		// 해결: 형제 노드를 Red로 변경하여 Black Height를 줄이고, 
		// 부모를 새로운 노드로 설정 후 다시 검사.
		if (sibling->Left()->GetColor() == Color::Black
			&& sibling->Right()->GetColor() == Color::Black)
		{
			sibling->SetColor(Color::Red);
			node = node->Parent();
			continue;
		}

		// Case3: 형제 노드 검정. 형제 노드의 오른쪽 자손이 빨간색.
		// 해결: 형제의 오른쪽 자식을 Black으로 변경, 
		// 형제를 Red로 변경 후 형제 기준 좌회전.
		if (sibling->Right()->GetColor() == Color::Red)
		{
			sibling->Right()->SetColor(Color::Black);
			sibling->SetColor(Color::Red);

			// 우회전.
			RotateToLeft(sibling);

			// 회전 후 형제 위치 업데이트.
			sibling = node->Parent()->Left();
		}

		// Case4: 형제 노드는 Black, 형제의 왼쪽 자식이 Red.
		// 해결: 형제를 부모와 같은 색으로 설정, 
		// 부모를 Black으로 변경, 
		// 형제의 왼쪽 자식을 Black으로 변경 후 부모 기준 우회전.
		if (sibling->Left()->GetColor() == Color::Red)
		{
			sibling->SetColor(sibling->Parent()->GetColor());
			sibling->Parent()->SetColor(Color::Black);
			sibling->Left()->SetColor(Color::Black);
			RotateToRight(node->Parent());
		}
	}

	// 루트 노드 블랙.
	root->SetColor(Color::Black);
}

void RedBlackTree::RotateToLeft(Node* node)
{
	// 오른쪽 자식 노드.
	Node* right = node->Right();

	// 오른쪽 자식 노드의 왼쪽 자식 노드를 부모의 오른쪽 자식으로 등록.
	node->SetRight(right->Left());

	// 하위 노드의 부모 변경 처리.
	if (right->Left() != nil)
	{
		right->Left()->SetParent(node);
	}

	// 오른쪽 자식 노드의 부모를 부모의 부모(조부모)로 설정.
	right->SetParent(node->Parent());

	// 부모가 root인 경우.
	if (right->Parent() == nullptr)
	{
		root = right;
	}

	// root가 아닐 때.
	else
	{
		// 조부모 기준 원래의 자식 위치로 복구.
		if (node == node->Parent()->Left())
		{
			node->Parent()->SetLeft(right);
		}
		else
		{
			node->Parent()->SetRight(right);
		}
	}

	// 좌회전 마무리.
	right->SetLeft(node);
	node->SetParent(right);
}

void RedBlackTree::RotateToRight(Node* node)
{
	// 왼쪽 자식 노드.
	Node* left = node->Left();

	// 왼쪽 자식 노드의 오른쪽 자식 노드를 부모의 왼쪽 자식으로 등록.
	node->SetLeft(left->Right());

	// 하위 노드의 부모 변경 처리.
	if (left->Right() != nil)
	{
		left->Right()->SetParent(node);
	}

	// 왼쪽 자식 노드의 부모를 부모의 부모(조부모)로 설정.
	left->SetParent(node->Parent());

	// 부모가 root인 경우.
	if (left->Parent() == nullptr)
	{
		root = left;
	}

	// root가 아닐 때.
	else
	{
		// 조부모 기준 원래의 자식 위치로 복구.
		if (node == node->Parent()->Left())
		{
			node->Parent()->SetLeft(left);
		}
		else
		{
			node->Parent()->SetRight(left);
		}
	}

	// 우회전 마무리.
	left->SetRight(node);
	node->SetParent(left);
}

Node* RedBlackTree::FindMinRecursive(Node* node)
{
	// 탈출 조건.
	if (node == nil)
	{
		return nullptr;
	}

	// 왼쪽 하위 노드가 더 이상 없으면 현재 노드 반환.
	if (node->Left() == nil)
	{
		return node;
	}

	return FindMinRecursive(node->Left());
}

Node* RedBlackTree::FindMaxRecursive(Node* node)
{
	// 탈출 조건.
	if (node == nil)
	{
		return nullptr;
	}

	// 오른쪽 하위 노드가 더 이상 없으면 현재 노드 반환.
	if (node->Right() == nil)
	{
		return node;
	}

	return FindMaxRecursive(node->Right());
}

void RedBlackTree::RemoveImpl(Node* node)
{
	// 삭제 대상 노드.
	Node* removed = nullptr;

	// 삭제할 위치의 대체 노드.
	Node* trail = nil;

	Node* target = node;

	// 자손이 하나 이하인 경우.
	if (target->Left() == nil || target->Right() == nil)
	{
		removed = target;

		// 자손이 하나만 있을 때.
		if (node->Left() != nil && node->Right() == nil)
		{
			trail = node->Left();
		}
		else if (node->Right() != nil && node->Left() == nil)
		{
			trail = node->Right();
		}
	}

	// 자손이 모두 있는 경우.
	else
	{
		// 대체할 노드 검색.
		// (왼쪽 하위 트리에서 가장 큰값을 대체 노드로 설정).
		removed = FindMaxRecursive(target->Left());

		// 대체 노드가 존재하면 해당 데이터 설정.
		//if (removed != nullptr && removed != nil)
		//{
		//	// 현재 노드의 값을 대상 노드의 값으로 변경.
		//	target->SetData(removed->Data());
		//}

		// 현재 노드의 값을 대상 노드의 값으로 변경.
		target->SetData(removed->Data());
	}

	// 대상 노드가 있는 경우.
	if (trail != nil /*&& trail != nullptr*/)
	{
		trail->SetParent(removed->Parent());
	}

	// removed 노드가 root인 경우.
	if (removed->Parent() == nullptr)
	{
		root = trail;
	}
	// root가 아닐 때.
	else
	{
		// trail 노드를 removed의 원래 위치로 설정.
		if (removed == removed->Parent()->Left())
		{
			removed->Parent()->SetLeft(trail);
		}
		else
		{
			removed->Parent()->SetRight(trail);
		}
	}

	// 재정렬 여부 확인후 진행.
	if (removed->GetColor() == Color::Black && trail != nil)
	{
		// 재정렬 진행.
		RestructureAfterRemove(trail);
	}

	// 노드 삭제.
	SafeDelete(removed);
}

void RedBlackTree::DestroyRecursive(Node* node)
{
	// 재귀 탈출 조건.
	if (node == nil)
	{
		return;
	}

	// 자식이 있는 경우.
	DestroyRecursive(node->Left());
	DestroyRecursive(node->Right());

	// 노드 삭제.
	SafeDelete(node);
}

void RedBlackTree::PrintRecursive(Node* node, int depth, int blackCount)
{
	// 탈출 조건.
	if (node == nil)
	{
		return;
	}

	// 노드 색상이 검정이면 blackCount 증가.
	if (node->GetColor() == Color::Black)
	{
		++blackCount;
	}

	// 부모 표기 문자열 설정.
	int parentData = 0;
	const char* position = "Root";

	// 부모 노드가 있는지 확인.
	if (node->Parent())
	{
		// 부모 노드의 데이터 저장.
		parentData = node->Parent()->Data();

		// 부모 노드로부터 현재 노드의 위치 설정.
		if (node == node->Parent()->Left())
		{
			position = "Left";
		}
		else
		{
			position = "Right";
		}
	}

	// 검은색 수 출력을 위한 문자열.
	char blackCountString[50] = { };

	// 자손이 없으면 현재까지의 검은색 노드 수 설정.
	if (node->Left() == nil && node->Right() == nil)
	{
		sprintf_s(blackCountString, "------ %d", blackCount);
	}

	// Depth 출력.
	for (int ix = 0; ix < depth; ++ix)
	{
		std::cout << "  ";
	}

	// 노드 색상에 따른 콘솔 설정.
	if (node->GetColor() == Color::Red)
	{
		SetTextColor(TextColor::Red);
	}
	else
	{
		SetTextColor(TextColor::White);
	}

	// 현재 노드 값 출력.
	std::cout
		<< node->Data() << " "
		<< node->ColorString() << " ["
		<< position << ", "
		<< parentData << "] " 
		<< blackCountString << "\n";

	// 노드를 출력한 뒤에는 콘솔 원래대로.
	SetTextColor(TextColor::White);

	// 하위 노드 출력.
	PrintRecursive(node->Left(), depth + 1, blackCount);
	PrintRecursive(node->Right(), depth + 1, blackCount);
}

void SetTextColor(TextColor color)
{
	static HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, (int)color);
}
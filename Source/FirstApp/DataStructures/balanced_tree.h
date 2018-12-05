#ifndef _BALANCED_TREE_H_
#define _BALANCED_TREE_H_

#include <iostream>
#include <stack>

namespace DataStructures
{
	template <typename T> class BNode;
	template <typename T> class BTree;
	template <typename T> class Connector
	{
	public:
		BNode<T>* next;
		BNode<T>* prev;
		Connector<T>* child;
		Connector<T>* parent;
	public:
		Connector() :
			next(nullptr),
			prev(nullptr),
			child(nullptr),
			parent(nullptr)
		{}

		~Connector()
		{
			if (nullptr != next)
				delete next;

			if (nullptr != child)
				delete child;

			next = nullptr;
			prev = nullptr;
			child = nullptr;
			parent = nullptr;
		}

		Connector<T>* InsertInto(BNode<T>* node, int(*cmp_func)(BNode<T>*, BNode<T>*))
		{
			Connector<T>* iter_conn = this;
			while (true)
			{
				BNode<T>* cmp = iter_conn->next;
				if (nullptr != cmp && cmp_func(node, cmp) < 0)
					iter_conn = GetNextConn(iter_conn);
				else
					break;
			}

			if (nullptr != iter_conn->child)
				return iter_conn->child;

			Connector<T>* conn = new Connector<T>();
			BTree<T>::Connect(node, conn);

			if (nullptr != iter_conn->next)
				BTree<T>::Connect(conn, iter_conn->next);

			BTree<T>::Connect(iter_conn, node);

			return this;
		}

		Connector<T>* ExistedNode(BNode<T>* node, int(*cmp_func)(BNode<T>*, BNode<T>*))
		{
			Connector<T>* iter_conn = this;
			while (true)
			{
				BNode<T>* cmp = iter_conn->next;
				if (nullptr != cmp && 0 == cmp_func(node, cmp))
					return iter_conn;
				if (nullptr != cmp && cmp_func(node, cmp) < 0)
					iter_conn = GetNextConn(iter_conn);
				else
					break;
			}

			if (nullptr != iter_conn->child)
				return iter_conn->child;

			return nullptr;
		}

		Connector<T>* GetParent()
		{
			return GetFirstConn(this)->parent;
		}

		bool IsFull(int size)
		{
			return CountNode() > size;
		}

		BNode<T>* SplitNode()
		{
			int count_node = CountNode();
			BNode<T>* cen_node = GetNodeIndex(count_node / 2 + 1);

			Connector<T>* left_conn = cen_node->prev;
			Connector<T>* right_conn = cen_node->next;

			cen_node->prev = nullptr;
			cen_node->next = nullptr;

			left_conn->next = nullptr;
			right_conn->prev = nullptr;

			Connector<T>* conn = new Connector<T>();
			BTree<T>::Connect(cen_node, conn);
			BTree<T>::LinkParent(conn, right_conn);

			return cen_node;
		}

		Connector<T>* GetOwning(Connector<T>* conn)
		{
			Connector<T>* iter = this;
			do
			{
				if (iter->child == conn)
					return iter;
				iter = GetNextConn(iter);
			} while (nullptr != iter);
		}

		static Connector<T>* GetNextConn(Connector<T>* conn) { return conn->next->next; }
		static Connector<T>* GetPrevConn(Connector<T>* conn) { return conn->prev->prev; }
		static Connector<T>* GetFirstConn(Connector<T>* conn)
		{
			Connector<T>* iter = conn;
			while (true)
			{
				if (nullptr != iter->prev)
					iter = GetPrevConn(iter);
				else
					break;
				conn = iter;
			}

			return conn;
		}
		static Connector<T>* GetLastConn(Connector<T>* conn)
		{
			Connector<T>* iter = conn;
			while (true)
			{
				if (nullptr != iter->next)
					iter = GetNextConn(iter);
				else
					break;
				conn = iter;
			}

			return conn;
		}


	private:
		int CountNode()
		{
			Connector<T>* iter = this;
			int count = 0;
			while (nullptr != iter->next)
			{
				count++;
				iter = GetNextConn(iter);
			}
			return count;
		}

		BNode<T>* GetNodeIndex(int index)
		{
			Connector<T>* iter = this;
			for (int i = 0; i < index; i++)
			{
				iter = GetNextConn(iter);
			}

			return iter->prev;
		}
	};

	template <typename T> class BNode
	{
	public:
		Connector<T>* next;
		Connector<T>* prev;
	public:
		BNode() :
			next(nullptr),
			prev(nullptr)
		{}

		virtual ~BNode()
		{
			if (nullptr != next)
				delete next;

			next = nullptr;
			prev = nullptr;
		}
	};

	template <typename T> class BTree
	{
	private:
		static const int size_node = 2;
		Connector<T>* root;
		int(*CompareFunc)(BNode<T>*, BNode<T>*);
	public:
		BTree(int(*cmp_func)(BNode<T>*, BNode<T>*)) :
			root(nullptr),
			CompareFunc(cmp_func)
		{}

		~BTree()
		{
			if (nullptr != root)
				delete root;
			root = nullptr;
		}

		void AddData(BNode<T>* node)
		{
			if (nullptr == root)
			{
				root = new Connector<T>();
				Connect(root, node);
				Connector<T>* tail = new Connector<T>();
				Connect(node, tail);
				return;
			}

			Connector<T>* iter_conn = root;
			do
			{
				Connector<T>* insert_conn = iter_conn->InsertInto(node, CompareFunc);
				if (insert_conn == iter_conn)
					break;
				else
					iter_conn = insert_conn;
			} while (true);

			while (true)
			{
				if (iter_conn->IsFull(size_node))
				{
					Connector<T>* parent = iter_conn->GetParent();
					if (nullptr == parent)
					{
						parent = new Connector<T>();
						LinkParent(parent, root);
						root = parent;
					}

					SplitNode(iter_conn);
					iter_conn = parent;
				}
				else
					break;
			}
		}

		static void SplitNode(Connector<T>* conn)
		{
			BNode<T>* cen_node = conn->SplitNode();
			Connector<T>* parent = conn->GetParent();
			if (nullptr == parent)
				throw std::exception("Failed");
			BNode<T>* right_node = parent->next;
			Connect(parent, cen_node);
			if (nullptr != right_node)
				Connect(cen_node->next, right_node);
		}

		static Connector<T>* MostLeft(Connector<T>* conn)
		{
			Connector<T>* ret = conn->child;
			if (nullptr == ret)
				return nullptr;

			Connector<T>* iter_ret = ret;
			while (nullptr != iter_ret)
			{
				ret = iter_ret;
				iter_ret = ret->child;
			}

			return ret;
		}

		static Connector<T>* MostRight(Connector<T>* conn)
		{
			Connector<T>* ret = conn->child;
			if (nullptr == ret)
				return nullptr;

			Connector<T>* iter_ret = ret;
			do
			{
				ret = Connector<T>::GetLastConn(iter_ret);
				iter_ret = ret->child;
			} while (nullptr != iter_ret);

			return ret;
		}

		BNode<T>* SearchData(BNode<T>* node)
		{
			if (nullptr == root)
				return nullptr;

			Connector<T>* iter_conn = root;
			do
			{
				iter_conn = iter_conn->ExistedNode(node, CompareFunc);
				if (nullptr == iter_conn)
					return nullptr;
				if (0 == CompareFunc(iter_conn->next, node))
					return iter_conn->next;
			} while (true);

			return nullptr;
		}

		void RemoveData(BNode<T>* node)
		{
			if (nullptr == node)
				return;

			//swap with most left node
			{
				Connector<T>* swap_conn = MostLeft(node->next);
				if (nullptr != swap_conn)
					SwapConnection(swap_conn->next, node);
			}

			//remove data
			Connector<T>* iter_conn = node->prev;
			Connect(node->prev, node->next->next);

			//re-balanced tree
			while (nullptr != iter_conn &&
				nullptr == iter_conn->prev &&
				nullptr == iter_conn->next)
			{
				MergeNode(iter_conn);
				iter_conn = Connector<T>::GetFirstConn(iter_conn)->parent;
			}

			if (nullptr == root->next)
			{
				Connector<T>* tmp = root->child;
				if (nullptr != root->child)
				{
					root->child->parent = nullptr;
					root->child = nullptr;
					delete root;
					root = tmp;
				}
				else
				{
					delete root;
					root = nullptr;
				}
			}
		}

		static void SwapConnection(BNode<T>* fnode, BNode<T>* snode)
		{
			Connector<T>* bfconn = fnode->prev;
			Connector<T>* afconn = fnode->next;

			Connector<T>* bsconn = snode->prev;
			Connector<T>* asconn = snode->next;

			Connect(bfconn, snode);
			Connect(snode, afconn);

			Connect(bsconn, fnode);
			Connect(fnode, asconn);
		}

		BNode<T>* GetExistedNode(BNode<T>* node)
		{
			Connector<T>* conn = root;
			if (nullptr == conn)
				return nullptr;
			while (nullptr != conn && nullptr != conn->next)
			{
				if (0 == CompareFunc(node, conn->next))
					return conn->next;

				if (CompareFunc(node, conn->next) < 0)
					conn = Connector<T>::GetNextConn(conn);
				else
					conn = conn->child;
			}

			return nullptr;
		}

		static void Connect(BNode<T>* node, Connector<T>* conn)
		{
			if (nullptr != node)
				node->next = conn;
			if (nullptr != conn)
				conn->prev = node;
		}

		static void Connect(Connector<T>* conn, BNode<T>* node)
		{
			if (nullptr != conn)
				conn->next = node;
			if (nullptr != node)
				node->prev = conn;
		}

		static void LinkParent(Connector<T>* parent, Connector<T>* child)
		{
			if (nullptr != parent)
				parent->child = child;
			if (nullptr != child)
				child->parent = parent;
		}

		static void MergeNode(Connector<T>* conn)
		{
			Connector<T>* parent = conn->GetParent();
			if (nullptr == parent)
				return;
			if (nullptr != parent->prev)
				MergeLeftNode(conn);
			else if(nullptr != parent->next)
				MergeRightNode(conn);
		}

		static void MergeLeftNode(Connector<T>* conn)
		{
			conn = Connector<T>::GetFirstConn(conn);

			Connector<T>* parent_conn = conn->GetParent();
			BNode<T>* parent_node = parent_conn->prev;
			Connector<T>* left_child = Connector<T>::GetPrevConn(parent_conn)->child;
			if (nullptr == left_child)
				throw std::exception("failed");

			left_child = Connector<T>::GetLastConn(left_child);

			parent_conn->child->parent = nullptr;
			parent_conn->child = nullptr;

			Connect(Connector<T>::GetPrevConn(parent_conn), parent_conn->next);

			Connect(left_child, parent_node);
			Connect(parent_node, conn);

			parent_conn->next = nullptr;
			parent_conn->prev = nullptr;
			delete parent_conn;

			conn = Connector<T>::GetFirstConn(conn);
			if (conn->IsFull(size_node))
				SplitNode(conn);
		}

		static void MergeRightNode(Connector<T>* conn)
		{
			Connector<T>* parent_conn = conn->GetParent();
			BNode<T>* parent_node = parent_conn->next;
			Connector<T>* next_parent_conn = Connector<T>::GetNextConn(parent_conn);
			Connector<T>* right_child = next_parent_conn->child;
			if (nullptr == right_child)
				throw std::exception("failed");

			next_parent_conn->child->parent = nullptr;
			next_parent_conn->child = nullptr;

			conn = Connector<T>::GetLastConn(conn);

			Connect(parent_conn, next_parent_conn->next);

			next_parent_conn->next = nullptr;
			next_parent_conn->prev = nullptr;
			delete next_parent_conn;

			Connect(conn, parent_node);
			Connect(parent_node, right_child);

			conn = Connector<T>::GetFirstConn(conn);
			if (conn->IsFull(size_node))
				SplitNode(conn);
		}
	};
}
#endif
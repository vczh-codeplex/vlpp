#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>

using namespace std;

struct Category
{
	string								name;
	int									soldPrice = 0;			// price that you want to buy one, if this is not a prior category
	int									fullPrice = 0;			// price that you want to build one without using any existing stuff
	int									displayPrice = 0;		// minimum price that you need to get a new one
	int									capacity = 0;

	vector<weak_ptr<Category>>			parents;
	vector<weak_ptr<Category>>			children;

	int CalculateDisplayPrice(map<string, int>& virtualCapacities)
	{
		auto it = virtualCapacities.find(name);
		if (it->second == 0)
		{
			if (soldPrice == -1)
			{
				int result = 0;
				for (auto child : children)
				{
					result += child.lock()->CalculateDisplayPrice(virtualCapacities);
				}
				return result;
			}
			else
			{
				return soldPrice;
			}
		}
		else
		{
			it->second--;
			return 0;
		}
	}

	void UpdateDisplayPrice(map<string, int> virtualCapacities)
	{
		if (displayPrice == 0)
		{
			displayPrice = CalculateDisplayPrice(virtualCapacities);
		}
	}

	void UpdateFullPrice()
	{
		if (fullPrice == 0)
		{
			if (soldPrice == -1)
			{
				for (auto c : children)
				{
					auto child = c.lock();
					child->UpdateFullPrice();
					fullPrice += child->fullPrice;
				}
			}
			else
			{
				fullPrice = soldPrice;
			}
		}
	}

	void Print(const string& indent)
	{
		cout << indent << name << " : " << displayPrice << "\t(full price: " << fullPrice << ")" << endl;
		string newIndent = indent + "  ";
		for (auto child : children)
		{
			child.lock()->Print(newIndent);
		}
	}
};

struct Shop
{
	map<string, shared_ptr<Category>>	categories;

	void AddCategory(const string& name, int price = -1)
	{
		auto category = make_shared<Category>();
		category->name = name;
		category->soldPrice = price;
		categories.insert(make_pair(name, category));
	}

	void AddChild(const string& parent, const string& child)
	{
		auto cparent = categories.find(parent)->second;
		auto cchild = categories.find(child)->second;

		if (cparent->soldPrice != -1)
		{
			throw invalid_argument("Parent should not have a price.");
		}

		cparent->children.push_back(cchild);
		cchild->parents.push_back(cparent);
	}

	bool Give(const string& name, int capacity)
	{
		auto category = categories.find(name)->second;
		int newCapacity = category->capacity + capacity;
		if (newCapacity < 0) return false;
		category->capacity = newCapacity;
		return true;
	}

	void UpdateDisplayPrice()
	{
		map<string, int> vc;
		for (auto pair : categories)
		{
			pair.second->displayPrice = 0;
			vc.insert(make_pair(pair.first, pair.second->capacity));
		}

		for (auto pair : categories)
		{
			pair.second->UpdateDisplayPrice(vc);
		}
	}

	void UpdateFullPrice()
	{
		for (auto pair : categories)
		{
			pair.second->fullPrice = 0;
		}
		for (auto pair : categories)
		{
			pair.second->UpdateFullPrice();
		}
	}

	void Print()
	{
		for (auto pair : categories)
		{
			if (pair.second->parents.size() == 0)
			{
				pair.second->Print("");
			}
		}
	}
};

int main()
{
	Shop shop;

	shop.AddCategory("A", 100);
	shop.AddCategory("B", 200);
	shop.AddCategory("D", 300);
	shop.AddCategory("E");
	shop.AddCategory("F");
	shop.AddCategory("G");

	shop.AddChild("G", "E");
	shop.AddChild("G", "F");
	shop.AddChild("E", "A");
	shop.AddChild("E", "B");
	shop.AddChild("F", "A");
	shop.AddChild("F", "D");
	
	shop.UpdateFullPrice();
	shop.UpdateDisplayPrice();
	shop.Print();

	cout << "=========================" << endl;
	shop.Give("E", 1);
	shop.Give("A", 1);
	shop.UpdateDisplayPrice();
	shop.Print();
	return 0;
}
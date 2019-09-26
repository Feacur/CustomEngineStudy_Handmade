#include <stddef.h> // offsetof

#include "demo_console/main_common.h"
#include "shared/collection_list.h"

struct Some_Struct {
	Link link_a;
	Link link_b;
	int data;
};

LIST_DECLARE(Some_Struct, link_a, list_a);
LIST_DECLARE(Some_Struct, link_b, list_b);

void print_list(cstring prefix, List<Some_Struct> &list) {
	printf("%s", prefix);
	for (auto it = list.base_link.next; it != &list.base_link; it = it->next)
	{
		auto node = list.LinkToNode(it);
		printf("%d, ", node->data);
	}
	printf("\n");
}

int main(int argc, char * argv[]) {
	auto s1 = new Some_Struct(); s1->data = 1;
	list_a.InsertHead(s1);
	list_b.InsertTail(s1);
	print_list("insert a 1 ||| ", list_a);
	print_list("list b     ||| ", list_b);
	printf("\n");

	auto s2 = new Some_Struct(); s2->data = 2;
	list_a.InsertHead(s2);
	list_b.InsertTail(s2);
	print_list("insert a 2 ||| ", list_a);
	print_list("list b     ||| ", list_b);
	printf("\n");

	auto s3 = new Some_Struct(); s3->data = 3;
	list_a.InsertHead(s3);
	list_b.InsertTail(s3);
	print_list("insert a 3 ||| ", list_a);
	print_list("list b     ||| ", list_b);
	printf("\n");

	delete s3;
	print_list("delete a 3 ||| ", list_a);
	print_list("list b     ||| ", list_b);
	printf("\n");

	auto s4 = new Some_Struct(); s4->data = 4;
	list_a.InsertHead(s4);
	list_b.InsertTail(s4);
	print_list("insert a 4 ||| ", list_a);
	print_list("list b     ||| ", list_b);
	printf("\n");

	auto s5 = new Some_Struct(); s5->data = 5;
	list_a.InsertHead(s5);
	list_b.InsertTail(s5);
	print_list("insert a 5 ||| ", list_a);
	print_list("list b     ||| ", list_b);
	printf("\n");

	delete s4;
	print_list("delete a 4 ||| ", list_a);
	print_list("list b     ||| ", list_b);
	printf("\n");

	list_a.Unlink(s1);
	print_list("unlink a 1 ||| ", list_a);
	print_list("list b     ||| ", list_b);
	printf("\n");

	list_a.Unlink(s5);
	print_list("unlink a 5 ||| ", list_a);
	print_list("list b     ||| ", list_b);
	printf("\n");

	delete s1;
	print_list("delete a 1 ||| ", list_a);
	print_list("list b     ||| ", list_b);
	printf("\n");

	delete s2;
	print_list("delete a 2 ||| ", list_a);
	print_list("list b     ||| ", list_b);
	printf("\n");

	delete s5;
	print_list("delete a 3 ||| ", list_a);
	print_list("list b     ||| ", list_b);
	printf("\n");
	
	getchar();
	return 0;
}

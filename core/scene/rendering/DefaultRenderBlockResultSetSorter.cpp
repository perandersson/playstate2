#include "../../Memory.h"
#include "DefaultRenderBlockResultSetSorter.h"
using namespace core;

DefaultRenderBlockResultSetSorter::DefaultRenderBlockResultSetSorter()
{
}

DefaultRenderBlockResultSetSorter::~DefaultRenderBlockResultSetSorter()
{
}

void DefaultRenderBlockResultSetSorter::Sort(RenderBlock** renderBlocks, uint32 count)
{
	if (count <= 2)
		return;

	QuickSort(renderBlocks, 0, count - 1);
}

void DefaultRenderBlockResultSetSorter::QuickSort(RenderBlock** items, int32 left, int32 right)
{
	int32 i = left, j = right;
	RenderBlock* tmp;
	uint32 pivot = items[(left + right) / 2]->id;

	while (i <= j) {
		while (items[i]->id < pivot)
			i++;
		while (items[j]->id > pivot)
			j--;
		if (i <= j) {
			tmp = items[i];
			items[i] = items[j];
			items[j] = tmp;
			i++;
			j--;
		}
	};

	if (left < j)
		QuickSort(items, left, j);
	if (i < right)
		QuickSort(items, i, right);
}

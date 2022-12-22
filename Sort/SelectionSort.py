def select_sort(lst):
    for i in range(len(lst) - 1):
        for j in range(i + 1, len(lst)):
            if lst[j] < lst[i]:
                lst[j], lst[i] = lst[i], lst[j]

arr = [5, 4, 4, 3, 1, 8]
select_sort(arr)
print(arr)


#include <stdio.h>

struct point //defining structure point with x and y coordinates
{
	int x;
	int y;
};

int main()
{
	int n; //number of points in set
	printf("Enter the number of points in the set : ");
	scanf("%d",&n); //input of n
	printf("\n");
	struct point a[n]; //defining strructur of array
	for (int i = 0; i < n; i++) //taking input of points
	{
		printf("Enter the coordinates of the point %d : ",i+1);
		scanf("%d %d",&a[i].x,&a[i].y); //x and y coordinates
		printf("\n");
	}
	printf("The maximal points : ");
	for (int i = 0; i < n; i++)
	{
		int maximal = 1;
		for (int j = 0; j < n; j++) //traversing over all elements for a given element
		{
			if (a[j].x > a[i].x && a[j].y > a[i].y) //dominating condition
			{
				maximal = 0; //setting maximal to 0
			}
		}
		if (maximal) //printing maximal values 
		{
			printf("(%d,%d),",a[i].x,a[i].y);
		}
	}
	printf("\n");
	return 0;
}

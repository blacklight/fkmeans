/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Test implementation for k-means library
 *
 *        Version:  1.0
 *        Created:  17/11/2010 16:01:08
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BlackLight (http://0x00.ath.cx), <blacklight@autistici.org>
 *        Licence:  GNU GPL v.3
 *        Company:  DO WHAT YOU WANT CAUSE A PIRATE IS FREE, YOU ARE A PIRATE!
 *
 * =====================================================================================
 */

#include	"kmeans.h"

#include	<alloca.h>
#include	<float.h>
#include	<limits.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<time.h>

#define 	DATASET_SIZE 	6
#define 	MIN_VAL 		0
#define 	MAX_VAL 		5

static const char colors[][10] = {
	"\033[0m", "\033[91m", "\033[92m", "\033[93m", "\033[94m", "\033[95m", "\033[96m", "\033[97m", 
	"\033[98m", "\033[99m", "\033[100m", "\033[101m", "\033[102m", "\033[103m", "\033[104m", "\033[105m", "\033[107m",
	"\033[1m", "\033[01;91m", "\033[01;92m", "\033[01;93m", "\033[01;94m", "\033[01;95m", "\033[01;96m", "\033[01;97m",
	"\033[01;98m", "\033[01;99m", "\033[01;100m", "\033[01;101m", "\033[01;102m", "\033[01;103m", "\033[01;104m", "\033[01;105m", "\033[01;107m"
};

/**
 * \brief  Give a bidimensional representations of a dataset (by dimension 2) and its clusters detected by the k-means algorithm
 * \param  km 	k-means object
 */

void
dataset_print ( kmeans_t *km )
{
	int i, j, k, l, found;
	double min_x = DBL_MAX,
		  min_y = DBL_MAX,
		  max_x = DBL_MIN,
		  max_y = DBL_MIN;

	/* If the dataset is not bidimensional, exit */
	if ( km->dataset_dim != 2 )
	{
		return;
	}

	for ( i=0; i < km->dataset_size; i++ )
	{
		if ( km->dataset[i][0] < min_x )
			min_x = km->dataset[i][0];

		if ( km->dataset[i][1] < min_y )
			min_y = km->dataset[i][1];

		if ( km->dataset[i][0] > max_x )
			max_x = km->dataset[i][0];

		if ( km->dataset[i][1] > max_y )
			max_y = km->dataset[i][1];
	}

	printf ( "+-" );

	for ( i = (int) min_y; i <= (int) max_y; i++ )
	{
		printf ( "--" );
	}

	printf ( "+\n" );

	for ( i = (int) min_x; i <= (int) max_x; i++ )
	{
		printf ( "| " );

		for ( j = (int) min_y; j <= (int) max_y; j++ )
		{
			found = 0;

			for ( k=0; k < km->k && !found; k++ )
			{
				for ( l=0; l < km->cluster_sizes[k] && !found; l++ )
				{
					if ( (int) km->clusters[k][l][0] == i &&
						(int) km->clusters[k][l][1] == j )
					{
						found = 1;
					}
				}
			}

			if ( found )
			{
				printf ( "%s* %s", ( k < sizeof ( colors )) ? colors[k] : colors[1], colors[0] );
			} else {
				printf ( "  " );
			}
		}

		printf ( "|\n" );
	}

	printf ( "+-" );

	for ( i = (int) min_y; i < (int) max_y + 1; i++ )
	{
		printf ( "--" );
	}

	printf ( "+\n\n" );
}		/* -----  end of function dataset_print  ----- */

/**
 * \brief Main function for the program
 */

int
main ( int argc, char *argv[] )
{
	int i, j, min_val, max_val, dataset_size;
	double **dataset;
	kmeans_t *km = NULL;

	switch ( argc )
	{
		case 1:
			min_val = MIN_VAL;
			max_val = MAX_VAL;
			dataset_size = DATASET_SIZE;
			break;

		case 2:
			min_val = MIN_VAL;
			max_val = MAX_VAL;
			dataset_size = atoi ( argv[1] );
			break;

		case 3:
			min_val = MIN_VAL;
			max_val = atoi ( argv[2] );
			dataset_size = atoi ( argv[1] );
			break;

		default:
			min_val = atoi ( argv[3] );
			max_val = atoi ( argv[2] );
			dataset_size = atoi ( argv[1] );
			break;
	}

	srand ( time ( NULL ));

	if ( !( dataset = (double**) alloca ( dataset_size * sizeof ( double* ))))
		return 1;

	for ( i=0; i < dataset_size; i++ )
	{
		if ( !( dataset[i] = (double*) alloca ( 2 * sizeof ( double ))))
			return 1;

		for ( j=0; j < 2; j++ )
		{
			dataset[i][j] = ( rand() % ( max_val - min_val )) + min_val;
		}
	}

	if ( !( km = kmeans_auto ( dataset, dataset_size, 2 )))
	{
		return 1;
	}

	dataset_print ( km );

	for ( i=0; i < km->k; i++ )
	{
		printf ( "Cluster %d: [ ", i );

		for ( j=0; j < km->cluster_sizes[i]; j++ )
		{
			printf ( "(%d, %d), ", (int) km->clusters[i][j][0], (int) km->clusters[i][j][1] );
		}

		printf ( "]\n" );
	}

	kmeans_free ( km );
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */


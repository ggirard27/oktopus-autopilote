#include "pathFinder.h"
#include <Arduino.h>

pathFinder::pathFinder()
{
}

pathFinder::~pathFinder()
{
  delete[] way_Coordinates;
  way_Coordinates = nullptr;
}

double **pathFinder::pathFinding(double start_point[][nCols], double end_point[][nCols], double external_boundaries[bordRows][nCols], double external_boundaries2[ileRows][nCols])
{  
    double combined_nodes[2 + bordRows + ileRows], visible_neighbours_library[2 + bordRows + ileRows][2 + bordRows + ileRows], shortest_path_array[nCols][2 + bordRows + ileRows], min_distance;
    int current_node_ID_index = 0, current_node_ID = 0, visitedNodes = 0, posCounter = 0, targetNodeID = 0, path_index = 0, path[2 + bordRows + ileRows];
    int *unvisited_nodes, *pathToTake;
    float cumulative_distance_to_current_node = 0, new_cumulative_distance = 0, previous_cumulative_distance_to_target_node = 0, distance_from_current_to_target_node = 0;
    double boundaries[bordRows + ileRows][nCols];
    double observerState[1][nCols];
    double currentTargetNode[1][nCols];
    double initial_combined_nodes[2 + bordRows + ileRows][nCols];
    int visibility = 0;
    
	// external boundaries
	for (int i = 0; i < nodeObstacle; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			boundaries[i][j] = external_boundaries[i][j];
		}
	}

	for (int i = nodeObstacle; i < (nodeObstacle + ileRows); i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			boundaries[i][j] = external_boundaries2[i - nodeObstacle][j];
		}
	}

	for (int i = (nodeObstacle + ileRows); i < (bordRows + ileRows); i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			boundaries[i][j] = external_boundaries[i - ileRows][j];
		}
	}

	// Create initial unvisited nodes array (missing 3rd row = infinity)
	initial_combined_nodes[0][0] = start_point[0][0];
	initial_combined_nodes[0][1] = start_point[0][1];

	for (int i = 0; i < (bordRows + ileRows); i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			initial_combined_nodes[i + 1][j] = boundaries[i][j];
		}
	}
	initial_combined_nodes[1 + bordRows + ileRows][0] = end_point[0][0];
	initial_combined_nodes[1 + bordRows + ileRows][1] = end_point[0][1];
    
	// Initialize vector infinity
	for (int i = 0; i < (bordRows + ileRows + 2); i++)
	{
		for (int j = 0; j < (bordRows + ileRows + 2); j++)
		{
			visible_neighbours_library[i][j] = NAN;
    }
		combined_nodes[i] = NAN;
	}

	// Create a library listing the visible neighbours of all of the nodes and their distances with respect to the reference nodes 
	for (int referenceNodeID = 0; referenceNodeID < (2 + bordRows + ileRows); referenceNodeID++)
	{
		for (int targetID = 0; targetID < (2 + bordRows + ileRows); targetID++)
		{
			//Assign observer and target nodes
			observerState[0][0] = initial_combined_nodes[referenceNodeID][0];
			currentTargetNode[0][0] = initial_combined_nodes[targetID][0];
			observerState[0][1] = initial_combined_nodes[referenceNodeID][1];
			currentTargetNode[0][1] = initial_combined_nodes[targetID][1];

			//Check visibility of target node from observer
			visibility = isLineOfSight.lineOfSight(observerState, currentTargetNode, boundaries);
			
			if (visibility == 1) // If target is visible
			{
				//Overwrite recorded distance
				combined_nodes[targetID] = sqrt(pow((currentTargetNode[0][0] - observerState[0][0]), 2) + pow((currentTargetNode[0][1] - observerState[0][1]), 2)); //double check variable init

				// Create a three dimensional array representing a library of the visible neighbours with respect to the reference node
				// The reference node ID is represented by the 'page' number
				visible_neighbours_library[referenceNodeID][targetID] = combined_nodes[targetID];
				combined_nodes[targetID] = NAN;
			}
		}
	}

	// Generate a list of all available nodes and assign them to the unvisited nodes set
	unvisited_nodes = new int[2 + bordRows + ileRows - visitedNodes];
	for (int index = 0; index < (2 + bordRows + ileRows); index++)
	{
		unvisited_nodes[index] = index;
	}

	// Copy the first 'page' of the visible_neighbours_library as the initial shortest path array
	for (int i = 0; i < (2 + bordRows + ileRows); i++)
	{
		shortest_path_array[0][i] = visible_neighbours_library[0][i];
		// Initialize all of the precursor nodes to 1 (the starting point)
		shortest_path_array[1][i] = 0;
	}
	// Take out the starting point(node 1) from the unvisited nodes set
	current_node_ID = 0;
	int *temp = new int[2 + bordRows + ileRows - visitedNodes];

	for (int i = 0; i< (2 + bordRows + ileRows - visitedNodes); i++)
	{
		if (unvisited_nodes[i] != current_node_ID)
		{
			temp[posCounter] = unvisited_nodes[i];
			posCounter = posCounter + 1;
		}
	}
	visitedNodes = 1;
	for (int i = 0; i< posCounter; i++)
	{
		unvisited_nodes[i] = temp[i];
	}
	delete[] temp;
	temp = nullptr;

	while (2 + bordRows + ileRows - visitedNodes > 0)
	{
		min_distance = shortest_path_array[0][0];
		double *cumulative_distances = new double[2 + bordRows + ileRows - visitedNodes];

		// Find the node with the smallest nonzero cumulative distance to be assigned as the next current node
		for (int i = 1; i < (2 + bordRows + ileRows - visitedNodes); i++)
		{
			cumulative_distances[i] = shortest_path_array[0][unvisited_nodes[i]];
			if ((min_distance > cumulative_distances[i] || min_distance == 0) && !isnan(cumulative_distances[i]) && cumulative_distances[i] != 0)
			{
				min_distance = cumulative_distances[i];
				current_node_ID_index = i;
			}
		}

		delete[] cumulative_distances;
		cumulative_distances = nullptr;

		// Extract the current node
		current_node_ID = unvisited_nodes[current_node_ID_index];

		// Take out the current node from the set of unvisited nodes
		//unvisited_nodes = setdiff(unvisited_nodes, current_node_ID);
		int *temp = new int[2 + bordRows + ileRows - visitedNodes];
		posCounter = 0;
		for (int i = 0; i < (2 + bordRows + ileRows - visitedNodes); i++)
		{
			if (unvisited_nodes[i] != current_node_ID)
			{
				temp[posCounter] = unvisited_nodes[i];
				posCounter = posCounter + 1;
			}
		}
		visitedNodes = visitedNodes + 1;
		for (int i = 0; i < posCounter; i++)
		{
			unvisited_nodes[i] = temp[i];
		}
		delete[] temp;

		// Refer to the library to find the distance to visible(neighboring) and UNVISITED nodes
		for (int i = 0; i < (2 + bordRows + ileRows - visitedNodes); i++)
		{
			// Assign one of the unvisited nodes as the target node
			targetNodeID = unvisited_nodes[i];

			// Visibility is implied if the distance recorded between the current and target nodes(in the library) is less than infinity
			if (!isnan(visible_neighbours_library[current_node_ID][targetNodeID]))
			{
				// Calculate the(tentative) cumulative distance for the target node
				cumulative_distance_to_current_node = shortest_path_array[0][current_node_ID];
				distance_from_current_to_target_node = visible_neighbours_library[current_node_ID][targetNodeID];
				new_cumulative_distance = cumulative_distance_to_current_node + distance_from_current_to_target_node;

				// Find the PREVIOUS cumulative distance to the target node
				previous_cumulative_distance_to_target_node = shortest_path_array[0][targetNodeID];

				// If a smaller cumulative distance from the starting node to the target node, passing through the current node was obtained
				if (new_cumulative_distance < previous_cumulative_distance_to_target_node || isnan(previous_cumulative_distance_to_target_node))
				{
					// Overwrite the previous cumulative distance to the smaller value
					shortest_path_array[0][targetNodeID] = new_cumulative_distance;

					// Replace the last precursor node with current_node_ID
					shortest_path_array[1][targetNodeID] = current_node_ID;
				}
			}
		}
	}

	delete[] unvisited_nodes;
	unvisited_nodes = nullptr;

	// Extract the shortest path
	//Assign the end point ID as the first entry on the path array
	path[path_index] = (sizeof(shortest_path_array) / sizeof(double) / 2) - 1;

	while (path[path_index] > 0) // Repeat until the starting node is reached
	{
		path_index = path_index + 1;
		path[path_index] = shortest_path_array[1][path[path_index - 1]];
	}

	pathToTake = new int[path_index];
	way_Coordinates = new double*[path_index + 1];
	for (int i = 0; i <= path_index + 1; ++i)
	{
		way_Coordinates[i] = new double[nCols];
	}
	for (int i = 0; i <= path_index; i++)
	{
		pathToTake[i + 1] = path[path_index - i];
		way_Coordinates[i + 1][0] = initial_combined_nodes[pathToTake[i + 1]][0];
		way_Coordinates[i + 1][1] = initial_combined_nodes[pathToTake[i + 1]][1];
	}
	// store sizeof(dynamic array) in first array 
	way_Coordinates[0][0] = path_index;

	return way_Coordinates;
}

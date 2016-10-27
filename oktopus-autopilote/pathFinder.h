#include "lineOfSight.h"

class pathFinder
{
	LoS isLineOfSight;

	public:
		pathFinder();
		double **pathFinding(double [][nCols], double[][nCols] , double[bordRows][nCols], double[ileRows][nCols]);

	private:
		double combined_nodes[2 + bordRows + ileRows], visible_neighbours_library[2 + bordRows + ileRows][2 + bordRows + ileRows], shortest_path_array[nCols][2 + bordRows + ileRows], min_distance;
		int current_node_ID_index, current_node_ID, visitedNodes, posCounter, targetNodeID, path_index, path[2 + bordRows + ileRows];
		int *unvisited_nodes, *pathToTake;
		float cumulative_distance_to_current_node, new_cumulative_distance, previous_cumulative_distance_to_target_node, distance_from_current_to_target_node;
		double boundaries[bordRows + ileRows][nCols];
		double observerState[1][nCols];
		double currentTargetNode[1][nCols];
		double initial_combined_nodes[2 + bordRows + ileRows][nCols];
		int visibility;
		double **way_Coordinates;
};
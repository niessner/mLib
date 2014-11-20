#ifndef CORE_BASE_DISTANCE_FIELD3_H_
#define CORE_BASE_DISTANCE_FIELD3_H_

namespace ml {

	template<class FloatType>
	class DistanceField3 : public Grid3<FloatType> {
	public:

		DistanceField3(const BinaryGrid3& grid) : Grid3(grid.dimX(), grid.dimY(), grid.dimZ()) {
			generateFromBinaryGrid(grid);
		}


	private:

		void generateFromBinaryGrid(const BinaryGrid3& grid) {

			BinaryGrid3 visited(grid.getDimensions());

			//initialize with grid distances
			for (size_t z = 0; z < grid.dimZ(); z++) {
				for (size_t y = 0; y < grid.dimY(); y++) {
					for (size_t x = 0; x < grid.dimX(); x++) {
						if (grid.isVoxelSet(x, y, z)) {
							(*this)(x, y, z) = (FloatType)0;
							visited.setVoxel(x, y, z);
						}
						else {
							(*this)(x, y, z) = std::numeric_limits<FloatType>::infinity();
						}
					}
				}
			}

			//initialize priority queue
			struct Voxel {
				Voxel(size_t _x, size_t _y, size_t _z, FloatType _d) : x(_x), y(_y), z(_z), dist(_d) {}

				bool operator<(const Voxel& other) const {
					return dist < other.dist;	//need to have the smallest one at 'top'; so it's inverted
				}

				size_t x, y, z;
				FloatType dist;
			};
			std::priority_queue<Voxel> queue;
			for (size_t z = 0; z < dimZ(); z++) {
				for (size_t y = 0; y < dimY(); y++) {
					for (size_t x = 0; x < dimX(); x++) {
						if (!grid.isVoxelSet(x, y, z)) {
							FloatType d;
							if (isNeighborSet(grid, x, y, z, d)) {
								queue.push(Voxel(x, y, z, d));
							}							
						}
					}
				}
			}

			while (!queue.empty()) {
				//first, check if the current voxel needs to be updated (and update if necessary)
				Voxel top = queue.top();
				queue.pop();

				if (!visited.isVoxelSet(top.x, top.y, top.z)) {

					visited.setVoxel(top.x, top.y, top.z);

					if (checkDistToNeighborAndUpdate(top.x, top.y, top.z)) {

						//second, check if neighbors need to be inserted into the queue
						for (size_t k = 0; k < 3; k++) {
							for (size_t j = 0; j < 3; j++) {
								for (size_t i = 0; i < 3; i++) {
									if (k == 1 && j == 1 && i == 1) continue;	//don't consider itself
									vec3ul n(top.x - 1 + i, top.y - 1 + j, top.z - 1 + k);
									if (isValidCoordinate(n.x, n.y, n.z) && !visited.isVoxelSet(n)) {
										FloatType d = (point3d<FloatType>((FloatType)top.x, (FloatType)top.y, (FloatType)top.z) -
													   point3d<FloatType>((FloatType)n.x, (FloatType)n.y, (FloatType)n.z)).length();
										FloatType dToN = (*this)(top.x, top.y, top.z)+d;
										if (dToN < (*this)(n.x, n.y, n.z)) {
											queue.push(Voxel(n.x, n.y, n.z, dToN));
										}
									}
								}
							}
						}

					}
				}				
			}

		}

		

		//! bools checks if there is a neighbor with a smaller distance (+ the dist to the current voxel); if then it updates the distances and returns true
		bool checkDistToNeighborAndUpdate(size_t x, size_t y, size_t z) {
			bool foundBetter = false;
			for (size_t k = 0; k < 3; k++) {
				for (size_t j = 0; j < 3; j++) {
					for (size_t i = 0; i < 3; i++) {
						if (k == 1 && j == 1 && i == 1) continue;	//don't consider itself
						vec3ul n(x - 1 + i, y - 1 + j, z - 1 + k);
						if (isValidCoordinate(n.x, n.y, n.z)) {
							FloatType d = (point3d<FloatType>((FloatType)x, (FloatType)y, (FloatType)z) -
											point3d<FloatType>((FloatType)n.x, (FloatType)n.y, (FloatType)n.z)).length();
							FloatType dToN = (*this)(n.x, n.y, n.z)+d;
							if (dToN < (*this)(x, y, z)) {
								(*this)(x, y, z) = dToN;
								foundBetter = true;
							}
						}
					}
				}
			}
			return foundBetter;

		}

		//! checks if a neighbor of (x,y,z) in the grid is set
		bool isNeighborSet(const BinaryGrid3& grid, size_t x, size_t y, size_t z, FloatType& d) const {
			for (size_t k = 0; k < 3; k++) {
				for (size_t j = 0; j < 3; j++) {
					for (size_t i = 0; i < 3; i++) {
						if (k == 1 && j == 1 && i == 1) continue;	//don't consider itself
						vec3ul v(x - 1 + i, y - 1 + j, z - 1 + k);
						if (grid.isValidCoordinate(v)) {
							if (grid.isVoxelSet(v)) {
								d = (point3d<FloatType>((FloatType)x, (FloatType)y, (FloatType)z) - 
									point3d<FloatType>((FloatType)v.x, (FloatType)v.y, (FloatType)v.z)).length();
								//TODO avoid the costly sqrt computation and just check for 1, sqrt(2), or, sqrt(3)
								return true;
							}
						}
					}
				} 
			}
			return false;
		}
	};

	typedef DistanceField3<float> DistanceField3f;
	typedef DistanceField3<double> DistanceField3d;
}



#endif // CORE_BASE_DISTANCE_FIELD3_H_
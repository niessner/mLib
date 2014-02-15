
#pragma once

template<class D> class LinearSolverEigen : public LinearSolver<D>
{
public:
	enum Method
	{
		LLT,
		LDLT,
		LU, //Inferior to LLT for symmetric problems
		QR, //Extremely slow
		ConjugateGradient_Diag,
		ConjugateGradient_LUT, //Doesn't always solve correctly
		BiCGSTAB_Diag,
		BiCGSTAB_LUT,
		Profile,
	};

	LinearSolverEigen(Method method = ConjugateGradient_Diag)
	{
		m_method = method;
	}

	Vector<D> solve(const SparseMatrix<D> &A, const Vector<D> &b)
	{
		MLIB_ASSERT(A.square() && b.size() == A.rows(), "invalid solve dimensions");
		Eigen::SparseMatrix<D> eigenMatrix;
		EigenUtility::makeEigenMatrix(A, eigenMatrix);
		return solve(eigenMatrix, b);
	}

	Vector<D> solve(const Eigen::SparseMatrix<D> &A, const Vector<D> &b)
	{
		Console::log() << "Solving using method: " << getMethodName(m_method) << std::endl;
		
		const Eigen::VectorXd bEigen = EigenUtility::makeEigenVector(b);
		Eigen::VectorXd x;

		if(m_method == LLT)
		{
			Eigen::SimplicialLLT< Eigen::SparseMatrix<double> > factorization(A);
			x = factorization.solve(bEigen);
		}
		else if(m_method == LDLT)
		{
			Eigen::SimplicialLDLT< Eigen::SparseMatrix<double> > factorization(A);
			x = factorization.solve(bEigen);
		}
		else if(m_method == LU)
		{
		    Eigen::SparseLU< Eigen::SparseMatrix<double> > factorization(A);
		    x = factorization.solve(bEigen);
		}
		else if(m_method == QR)
		{
			Eigen::SparseQR< Eigen::SparseMatrix<double>, Eigen::COLAMDOrdering<int> > factorization(A);
			x = factorization.solve(bEigen);
		}
		else if(m_method == ConjugateGradient_Diag)
		{
			Eigen::ConjugateGradient< Eigen::SparseMatrix<double>, Eigen::Lower, Eigen::DiagonalPreconditioner<double > > solver;
			solver.setTolerance(1e-10);
			solver.compute(A);
			x = solver.solve(bEigen);
			//Console::WriteLine("Iterations: " + String(solver.iterations()));
			//Console::WriteLine("Error: " + String(solver.error()));
		}
		else if(m_method == BiCGSTAB_Diag)
		{
			Eigen::BiCGSTAB< Eigen::SparseMatrix<double>, Eigen::DiagonalPreconditioner<double > > solver;
			solver.setTolerance(1e-10);
			solver.compute(A);
			x = solver.solve(bEigen);
			//Console::WriteLine("Iterations: " + String(solver.iterations()));
			//Console::WriteLine("Error: " + String(solver.error()));
		}
		else if(m_method == BiCGSTAB_LUT)
		{
			Eigen::BiCGSTAB< Eigen::SparseMatrix<double>, Eigen::IncompleteLUT<double > > solver;
			solver.setTolerance(1e-10);
			solver.compute(A);
			x = solver.solve(bEigen);
			//Console::WriteLine("Iterations: " + String(solver.iterations()));
			//Console::WriteLine("Error: " + String(solver.error()));
		}
		/*else if(m_method == Profile)
		{
			Console::WriteLine("Profiling");
			const int methodCount = (int)Profile;
			Vector< Vector<double> > results(methodCount);
			for(int methodIndex = 0; methodIndex < methodCount; methodIndex++)
			{
				results[methodIndex] = solve(A, b, (Method)methodIndex);
				if(methodIndex != 0)
				{
					double maxDeviation = 0.0;
					for(UINT variableIndex = 0; variableIndex < b.Length(); variableIndex++) maxDeviation = Math::Max(maxDeviation, fabs(results[methodIndex][variableIndex] - results[0][variableIndex]));
					Console::WriteLine("Max deviation from LLT: " + String(maxDeviation));
				}
			}
		}*/
		else
		{
			MLIB_ERROR("Unknown method");
		}

		return EigenUtility::dumpEigenVector<D>(x);
	}

	Vector<D> solveLeastSquares(const SparseMatrix<D> &A, const Vector<D> &b)
	{
		//return solve(A.transpose() * A, b);

		Eigen::SparseMatrix<D> eigenMatrix;
		EigenUtility::makeEigenMatrix(A, eigenMatrix);
		return solveLeastSquares(eigenMatrix, b);
	}

	Vector<D> solveLeastSquares(const Eigen::SparseMatrix<D> &A, const Vector<D> &b)
	{
		Console::log("Solving least-squares problem using QR");

		const Eigen::VectorXd bEigen = EigenUtility::makeEigenVector(b);
		Eigen::SparseQR< Eigen::SparseMatrix<D>, Eigen::COLAMDOrdering<int> > factorization(A);
		Eigen::VectorXd x = factorization.solve(bEigen);

		return EigenUtility::dumpEigenVector<D>(x);
	}

private:
	static String getMethodName(Method m)
	{
		switch(m)
		{
		case LLT: return "LLT";
		case LDLT: return "LDLT";
		case LU: return "LU";
		case QR: return "QR";
		case ConjugateGradient_Diag: return "ConjugateGradient_Diag";
		case ConjugateGradient_LUT: return "ConjugateGradient_LUT";
		case BiCGSTAB_Diag: return "BiCGSTAB_Diag";
		case BiCGSTAB_LUT: return "BiCGSTAB_LUT";
		case Profile: return "Profile";
		default: return "Unknown";
		}
	}

	Method m_method;
};

# SRGN

Simulation-level fit based on Reweighting Generator-level events with Neural networks  (SRGN, pronounced “surgeon”) is a technique for parameter estimation in the presence of detector effects introduced in [arXiv:2010.03569](https://arxiv.org/abs/2010.03569); it is an extension of the DCTR protocol from [arXiv:1907.08209](https://arxiv.org/abs/1907.08209).

## Notebooks
We provide a few example notebooks to illustrate how SRGN works.

### Gaussian Example
First, simple Gaussian examples are used, where the probability density is known and thus the reweighting function can be computed analytically. The fundemental features of SRGN are explored with these examples.
- Fitting the mean $\mu$ of a Gaussian: `./Gaussian-Example/1D-Gaussian.ipynb`
- Fitting both the mean $\mu$ and standard deviation $\sigma$ of a Gaussian: `./Gaussian-Example/2D-Gaussian.ipynb`
- An examination of a generator parameter dependent detector response: `./Gaussian-Example/1D-Gaussian-GeneratorParameterDependentDetectorResponse.ipynb`

### Parton Shower Monte Carlo Tuning
The parton shower examples from the DCTR paper ([arXiv:1907.08209](https://arxiv.org/abs/1907.08209)) are used as a second example. These examples show how the new method can be effective with high-dimensional features, but do not incorporate detector effects.
- $\texttt{TimeShower:alphaSvalue}$: `./Parton-Shower-MC-Tuning/SRGN_1D_alphaS.ipynb`
- $\texttt{StringZ:aLund}$: `./Parton-Shower-MC-Tuning/SRGN_1D_aLund.ipynb`
- $\texttt{StringFlav:probStoUD}$: `./Parton-Shower-MC-Tuning/SRGN_1D_probStoUD.ipynb`

### Top Quark Mass
Finally, a measurement of the top quark mass is used as a third example to demonstrate both multivariate fitting and detector effects: `./Top-Quark-Mass.ipynb`

## Sample Datasets
The datasets used to run the example notebooks can be found on Zenodo. Specficially, the datasets for the Parton Shower example can be found at https://zenodo.org/record/3518708, and the datasets used for the Top Quark Mass example can be found at https://zenodo.org/record/4067673.

from typing import List, Tuple
import numpy as np

# Матрица выигрыша представляется объектом
# np.ndarray

def nash_equilibrium(payoff_matrix: np.ndarray) -> \
        Tuple[float, np.ndarray, np.ndarray]:    
    """
    Solves a matrix game with the given "payoff_matrix" matrix
    
    Parameters:
    - payoff_matrix (np.ndarray): The matrix representing the game
    
    Returns:
    Tuple[float, np.ndarray, np.ndarray]: 
    A tuple containing the game value, the 1st player's strategy,
    and the 2nd player's strategy.
    """
    # TODO Написать функцию
    pass


def visualize(matrix: np.ndarray, spectrum: Tuple[np.ndarray, np.ndarray]):
    """
    Visualizes the spectrum of optimal strategies
    of the matrix game with the "spectrum" spectrum
    
    Parameters:
    - matrix (np.ndarray): payoff matrix for the game
    - spectrum (Tuple[np.ndarray, np.ndarray]): 
    spectrum of optimal strategies represented by a tuple,
    where the first element is the 1st player's strategies and 
    the second element is the 2nd player's strategies
    
    Returns:
    None
    """
    import matplotlib.pyplot as plt
    import seaborn as sns
    
    # Визуализируем хитмап
    
    fig = plt.figure()
    plt.title('Heatmap for the game matrix')
    probability_matrix = spectrum[0][:, None] * spectrum[1]
    sns.heatmap(probability_matrix, annot=matrix, 
                fmt='d', linecolor='black', linewidth=0.01,
                annot_kws={'size': 13},
                cbar_kws={'label': 'probability'},
                xticklabels = [x for x in range(1, len(spectrum[1]) + 1)],
                yticklabels = [y for y in range(1, len(spectrum[0]) + 1)])
    
    # А теперь визуализируем спектры стратегий
    fig = plt.figure(figsize=(14,7))
    plt.subplot(1, 2, 1)
    plt.ylabel('probability')
    for k in [0,1]:
        ax = plt.subplot(1, 2, k + 1)
        plt.xlabel('strategy #')
        plt.title(f'Optimal strategies of the player #{k + 1}')
        for i in range(1,len(spectrum[k]) + 1):
            plt.plot([i, i], [0, spectrum[k][i - 1]],
                     c=plt.cm.plasma(spectrum[k][i - 1] / 1.01))
        plt.scatter([x for x in range(1, len(spectrum[k]) + 1)], 
                    spectrum[k], s=100, cmap='plasma', c=spectrum[k], vmax = 1)
        plt.xticks(range(1, len(spectrum[k]) + 1))
        plt.yticks(np.linspace(0, 1, 26))
        ax.set_xlim(1 - 0.1, len(spectrum[k]) + 0.1)
        ax.set_ylim(0 - 0.01, max(spectrum[k]) + 0.05)
        plt.grid(axis='y')
    plt.show()

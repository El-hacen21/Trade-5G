import pandas as pd

def aggregate_data(file_path, output_file_path):
    # Define the column names
    columns = ['nb_nodes', 'scenario_type', 'seed', 'duration', 
               'average_total_offers_delay', 'average_total_demand_delay',
               'txFinality_tmp_normal', 'count_offers_quantity_ratio',
               'count_demands_quantity_ratio', 'expired_offers_quantity_ratio', 'utlisation_ratio', 'malicious',
                 'attack_ratio', 'commission', 'seller_profit', 'buyer_profit', 'nsp_profit', 'average_price']

    # Read the data, assuming no header in the file
    data = pd.read_csv(file_path, header=None, names=columns)

    # Group by scenario, duration, and number of nodes
    grouped_data = data.groupby(['scenario_type', 'duration', 'nb_nodes', 'malicious', 'commission']).mean(numeric_only=True)


    # Reset index so that scenario_type, duration, and nb_nodes become columns
    grouped_data.reset_index(inplace=True)

    # Save the aggregated data to a new file
    grouped_data.to_csv(output_file_path, index=False)

    return grouped_data

# Example usage
file_path = '../results/latency.csv'  # Replace with your input file path
output_file_path = '../plot/clean_data.csv'  # Replace with your desired output file path
aggregated_results = aggregate_data(file_path, output_file_path)
print(aggregated_results)

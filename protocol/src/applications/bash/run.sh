#!/bin/bash

# Script to run the Trade-5G simulator with various parameter combinations

# Parameters
OUTPUT_DIR="../results"
SIMULATOR="../model/./simulator"
LATENCY_FILE="$OUTPUT_DIR/latency.csv"
LOG_FILE="$OUTPUT_DIR/logs_f.txt"

# Cleanup previous results
rm -f "$LATENCY_FILE" # Will be recreated by the simulator
rm -f "$LOG_FILE"
touch "$LOG_FILE"

# Parameter ranges
SCENARIOS=(1)              # List of scenarios
N_VALUES=(4 10 16 22 28)   # Number of robots
MALICIOUS_VALUES=(1)       # Number of malicious nodes
COMMISSION_VALUES=(10)     # Commission percentage
DURATION_VALUES=(1000)     # Simulation duration
SEEDS=(0 1 2 3 4)          # Random seeds

# Run the simulations
for SCENARIO in "${SCENARIOS[@]}"; do
    for N in "${N_VALUES[@]}"; do
        for MALICIOUS in "${MALICIOUS_VALUES[@]}"; do
            for COMMISSION in "${COMMISSION_VALUES[@]}"; do
                for DURATION in "${DURATION_VALUES[@]}"; do
                    for SEED in "${SEEDS[@]}"; do
                        echo "Running: scenario=$SCENARIO, n=$N, seed=$SEED, malicious=$MALICIOUS, commission=$COMMISSION, duration=$DURATION"
                        "$SIMULATOR" --nb_robots="$N" --scenario="$SCENARIO" --seed="$SEED" --malicious="$MALICIOUS" --commission="$COMMISSION" --duration="$DURATION" | tee -a "$LOG_FILE"
                    done
                done
            done
        done
    done
done

echo "All simulations completed!"

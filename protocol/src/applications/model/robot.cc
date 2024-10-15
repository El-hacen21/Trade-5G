
// #include "hello.h"
#include "robot.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/network-module.h"
#include "utils.h"
// #include <random>

using namespace std;
namespace ns3
{

	NS_LOG_COMPONENT_DEFINE("Robot");

	NS_OBJECT_ENSURE_REGISTERED(Robot);

	TypeId Robot::GetTypeId(void)
	{
		static TypeId tid =
			TypeId("ns3::Robot").SetParent<Application>().SetGroupName("Applications").AddConstructor<Robot>();
		return tid;
	}

	Robot::~Robot()
	{
	}

	Robot::Robot()
	{
	}

	void Robot::SetUp(Address address, uint16_t port, NodeContainer robots, RobotParameter parameters, map<uint32_t, RobotStatics> *stats)
	{
		m_address = address;
		m_port = port;
		m_robots = robots;
		m_stats = stats;
		m_paramerters = parameters;
		m_resources = m_paramerters.INITIAL_RESOURCES_QUANTITY;

		auto commission = ((m_paramerters.INITIAL_PRICE * m_paramerters.commission) / 100);
		m_c1 = commission * 0.7;
		m_c2 = commission * 0.3;
		// srand(m_paramerters.seed);
	}

	// void Robot::GenerateTransaction()
	// {
	// 	// m_generator(rand());

	// 	// std::uniform_int_distribution<> uniform_dist(0, 1);

	// 	// // Price: Log-Normal Distribution
	// 	// std::lognormal_distribution<double> priceDistribution(m_paramerters.PRICE_MEAN, m_paramerters.PRICE_STD_DEV);

	// 	// // Quantity: Poisson Distribution
	// 	// std::poisson_distribution<int> quantityDistribution(m_paramerters.QUANTITY_LAMBDA);

	// 	// // Tx arrival time: Exponential Distribution
	// 	// std::exponential_distribution<double> durationDistribution(m_paramerters.DURATION_LAMBDA_EXP);
	// 	// std::exponential_distribution<double> lifetimeDistribution(m_paramerters.LIFE_TIME_LAMBDA);

	// 	double meanOfferPrice = 100.0; // Example mean price for offers
	// 	double meanDemandPrice = 95.0; // Example mean price for demands
	// 	double priceStdDev = 10.0;	   // Standard deviation for price
	// 	// uniform_int_distribution<int> offerQuantityDistribution(1, 20); // Quantity range for offers
	// 	uniform_int_distribution<int> quantityDistribution(1, 10);							   // Quantity range for demands
	// 	exponential_distribution<double> lifetimeDistribution(m_paramerters.LIFE_TIME_LAMBDA); // Lifetime distribution

	// 	double currentTime = 0;

	// 	while (currentTime <= m_paramerters.duration)
	// 	{
	// 		currentTime += durationDistribution(m_generator);
	// 		double price = priceDistribution(m_generator);
	// 		int quantity = quantityDistribution(m_generator);
	// 		double duration = lifetimeDistribution(m_generator);

	// 		string hash = std::to_string(m_robotId) + to_string(currentTime);

	// 		// Adjusted strategy based on market scenario
	// 		switch (m_paramerters.scenario_type)
	// 		{
	// 		case HIGH_DEMAND_LOW_OFFER:
	// 			GenerateTransactionBasedOnRatio(1, 5, hash, quantity, duration, price, currentTime);
	// 			break;
	// 		case LOW_DEMAND_HIGH_OFFER:
	// 			GenerateTransactionBasedOnRatio(4, 5, hash, quantity, duration, price, currentTime);
	// 			break;
	// 		case RANDOM:
	// 			if (uniform_dist(m_generator) == 0)
	// 				double price = normal_distribution<double>(meanDemandPrice, priceStdDev)(m_generator);
	// 			Simulator::Schedule(Seconds(currentTime), &Robot::GenerateDemand, this, hash, quantity, duration, price);
	// 			else double price = normal_distribution<double>(meanOfferPrice, priceStdDev)(m_generator);
	// 			Simulator::Schedule(Seconds(currentTime), &Robot::GenerateOffer, this, hash, quantity, duration, price);
	// 			break;
	// 		}
	// 	}
	// 	std::cout << "End " << currentTime << std::endl;
	// }

	void Robot::GenerateTransaction()
	{
		double currentTime = 0;
		std::exponential_distribution<double> durationDistribution(m_paramerters.DURATION_LAMBDA_EXP);

		while (currentTime <= m_paramerters.duration)
		{
			currentTime += durationDistribution(m_generator); // Generate the time for the next transaction
			// cout << m_robotId << " current time : " << currentTime << endl;
			// Generate transaction based on the scenario
			switch (m_paramerters.scenario_type)
			{
			case HIGH_DEMAND_LOW_OFFER:
				GenerateTransactionBasedOnRatio(1, 5, currentTime);
				break;
			case LOW_DEMAND_HIGH_OFFER:
				GenerateTransactionBasedOnRatio(4, 5, currentTime);
				break;
			case RANDOM:
				GenerateRandomTransaction(currentTime);
				break;
			}
		}
	}

	void Robot::GenerateTransactionBasedOnRatio(int offerProbability, int totalProbability, double currentTime)
	{
		std::uniform_int_distribution<> decisionDist(1, totalProbability);
		if (decisionDist(m_generator) <= offerProbability)
			GenerateO(currentTime);
		else
			GenerateD(currentTime);
	}

	void Robot::GenerateRandomTransaction(double currentTime)
	{
		std::uniform_int_distribution<> uniform_dist(0, 1);
		if (uniform_dist(m_generator) == 0)
			GenerateO(currentTime);
		else
			GenerateD(currentTime);
	}

	double Robot::ComputeAdjustedPriceOffer(double currentTime, double P_initial, double P_market, double alpha, double beta)
	{
		auto decay_rate = 1 - (currentTime / (double)m_paramerters.duration);
		auto ajustedPrice = P_market * decay_rate - m_c2;

		return ajustedPrice;
	}

	double Robot::ComputeAdjustedPriceDemand(double currentTime, double P_initial, double P_market, double alpha, double beta)
	{
		// // If P_market is 0, set alpha to 1
		// if (P_market == 0)
		// {
		// 	alpha = 1;
		// }

		// // auto decay_rate = GetSeconds() / (double)m_paramerters.duration;
		// auto ajustedPrice = (P_initial * (1 - currentTime / (double)m_paramerters.duration)) * alpha + (P_market * beta);

		// ajustedPrice = std::normal_distribution<double>(ajustedPrice, priceStdDev)(m_generator);

		// ajustedPrice = min(max(ajustedPrice, (m_paramerters.INITIAL_PRICE * (m_paramerters.commission) / 100)), P_initial);
		// cout << "Adjusted price: " << ajustedPrice << endl;

		auto decay_rate = 1 - (currentTime / (double)m_paramerters.duration);
		auto ajustedPrice = m_paramerters.INITIAL_PRICE * decay_rate - m_c2;

		return ajustedPrice;
	}

	void Robot::GenerateO(double currentTime)
	{
		int quantity = offerQuantityDistribution(m_generator);
		double lifetime = lifetimeDistribution(m_generator);
		// double lifetime = m_paramerters.duration;
		std::string hash = std::to_string(m_robotId) + std::to_string(currentTime);

		Simulator::Schedule(Seconds(currentTime), &Robot::GenerateOffer, this, hash, quantity, lifetime);
	}

	void Robot::GenerateD(double currentTime)
	{
		// double price = std::normal_distribution<double>(meanDemandPrice, priceStdDev)(m_generator);

		int quantity = demandQuantityDistribution(m_generator);
		double lifetime = lifetimeDistribution(m_generator);
		// double lifetime = m_paramerters.duration;
		std::string hash = std::to_string(m_robotId) + std::to_string(currentTime);
		Simulator::Schedule(Seconds(currentTime), &Robot::GenerateDemand, this, hash, quantity, lifetime);
	}

	void Robot::GenerateDemand(const string &hash, int quantity, double duration)
	{
		auto market_state = ComputeAveragePrice(m_unmatched_offers);
		auto toal_demands_quantity = ComputeTotalQuantity(m_unmatched_demands);

		auto market_mean = market_state.first == 0 ? m_paramerters.INITIAL_PRICE : market_state.first;

		auto price = ComputeMarketPriceDemands(market_mean, toal_demands_quantity, market_state.second);
		// auto price = 8;
		// double price = ComputeAdjustedPriceDemand(GetSeconds(), m_paramerters.MEAN_PRICE_DEMANDS, market_price, m_paramerters.ALPHA, m_paramerters.BETA);

		if (price < m_c2)
		{
			return;
		}

		cout << GetSeconds() << "--->Demand price price : " << price << endl;

		// cout << "Byer price : " << price << " Market price : " << market_price << " Quantities : " << toal_demands_quantity << "/" << market_state.second << endl;

		count_demands += quantity;
		auto tx = make_shared<Transaction>(hash, m_robotId, TRADE_TYPE::DEMAND, RESSOURCE_TYPE::BANDWIDTH, quantity, duration, price);
		tx->SetTimeCreated(GetSeconds());
		// tx->SetSourceId(m_robotId);
		// tx->SetTxHash(hash);

		m_mempool.AddTransaction(tx);
		// Broadcast to all peers

		auto pcket_type = MyPacket::TX;
		string msg = tx->Serialize();
		string msg_sig = "";
		auto packet = MyPacket(pcket_type, msg, m_robotId,
							   msg_sig, msg.size(), GetSeconds());
		Broadcast(packet);
	}

	double Robot::ComputeMarketPriceDemands(const double market_mean, const double demands_q, const double offers_q) const
	{
		auto decay_rate = 1 - (GetSeconds() / (double)m_paramerters.duration);
		auto ajustedPrice = m_paramerters.INITIAL_PRICE * decay_rate;

		double offers_demand_ratio = (demands_q + m_paramerters.EPSILON) / (offers_q + m_paramerters.EPSILON);

		auto result = max(market_mean * (1 + m_paramerters.DELTA * (offers_demand_ratio - 1)), ajustedPrice);

		return result - m_c2;
	}

	double Robot::ComputeMarketPriceOffers(const double market_mean, const double demands_q, const double offers_q) const
	{
		auto decay_rate = 1 - (GetSeconds() / (double)m_paramerters.duration);
		auto ajustedPrice = m_paramerters.INITIAL_PRICE * decay_rate;

		double offers_demand_ratio = (demands_q + m_paramerters.EPSILON) / (offers_q + m_paramerters.EPSILON);

		auto result = min(market_mean * (1 + m_paramerters.DELTA * (offers_demand_ratio - 1)), ajustedPrice);

		return result - m_c2;
	}

	pair<double, double> Robot::ComputeAveragePrice(priority_queue<shared_ptr<Transaction>, vector<shared_ptr<Transaction>>, TransactionComparator> &pq)
	{
		if (pq.empty())
		{
			return make_pair(0.0, 0.0);
		}

		double sum = 0.0;
		double quality = 0.0;
		int count = 0;
		vector<shared_ptr<Transaction>> temp;

		while (!pq.empty())
		{
			shared_ptr<Transaction> transaction = pq.top();
			pq.pop();
			temp.push_back(transaction);
			sum += transaction->GetPrice();
			quality += transaction->GetQuantity();
			++count;
		}

		// Reinsert transactions into the priority queue
		for (const auto &transaction : temp)
		{
			pq.push(transaction);
		}

		return make_pair(sum / (double)count, quality);
	}

	double Robot::ComputeTotalQuantity(priority_queue<shared_ptr<Transaction>, vector<shared_ptr<Transaction>>, TransactionComparator> &pq)
	{

		double quality = 0.0;
		vector<shared_ptr<Transaction>> temp;

		while (!pq.empty())
		{
			shared_ptr<Transaction> transaction = pq.top();
			pq.pop();

			if (!transaction->IsExpired(GetSeconds()))
			{
				temp.push_back(transaction);
				quality += transaction->GetQuantity();
			}
		}

		// Reinsert transactions into the priority queue
		for (const auto &transaction : temp)
		{
			pq.push(transaction);
		}

		return quality;
	}

	void Robot::GenerateOffer(const string &hash, int quantity, double duration)
	{
		auto market_state = ComputeAveragePrice(m_unmatched_demands);
		auto toal_offers_quantity = ComputeTotalQuantity(m_unmatched_offers);

		auto market_mean = market_state.first == 0 ? m_paramerters.INITIAL_PRICE : market_state.first;
		// cout << "Market Mean : " << market_mean << endl;
		auto price = ComputeMarketPriceOffers(market_mean, market_state.second, toal_offers_quantity);

		// auto price = 8;

		// double price = ComputeAdjustedPriceOffer(GetSeconds(), m_paramerters.MEAN_PRICE_OFFERS, market_price, m_paramerters.ALPHA, m_paramerters.BETA);

		if (price <= m_c1 + m_c2)
		{
			return;
		}
		cout << GetSeconds() << "--->Offer price price : " << price << endl;
		//
		// cout << "-->Market Price : " << market_price << " Market mean " << market_mean << endl;
		// cout << "Offer price : " << price << " Market price : " << market_price << " Quantities : " << market_state.second << "/" << toal_offers_quantity << endl;

		if (quantity <= 0)
		{
			return; // No enough ressource to put on the market
		}

		// You cannot offer more than you have!
		quantity = min(quantity, m_resources);
		DecreaseResources(quantity);

		count_offers += quantity;

		// cout << "Offer generated  at :  " << GetSeconds() << endl;
		auto tx = make_shared<Transaction>(hash, m_robotId, TRADE_TYPE::OFFER, RESSOURCE_TYPE::BANDWIDTH, quantity, duration, price);
		tx->SetTimeCreated(GetSeconds());
		// tx->SetSourceId(m_robotId);
		// tx->SetTxHash(hash);

		// cout << "hash : " << tx.GetTxHash() << endl;

		// cout << " offer " << *tx << endl;

		m_mempool.AddTransaction(tx);

		// Broadcast to all peers

		auto pcket_type = MyPacket::TX;
		string msg = tx->Serialize();
		string msg_sig = "";
		auto packet = MyPacket(pcket_type, msg, m_robotId,
							   msg_sig, msg.size(), GetSeconds());

		Broadcast(packet);
		// DecreaseResources(quantity);
	}

	void Robot::DoDispose(void)
	{
		m_socket = 0;

		while (!m_unmatched_demands.empty())
		{
			auto demand = m_unmatched_demands.top();
			// expired_demands_quantity += demand->GetQuantity();
			// count_expired_demands++;
			m_expired_demands.AddTransaction(demand);
			m_unmatched_demands.pop();
		}

		while (!m_unmatched_offers.empty())
		{
			auto offer = m_unmatched_offers.top();
			// expired_offers_quantity += offer->GetQuantity();
			// count_expired_offers++;
			m_expired_offers.AddTransaction(offer);
			m_unmatched_offers.pop();
		}

		// Results
		if (m_robotId == 0)
		{
			double averageExpiredDemandsPrice = 0;
			double averageExpiredDemandsDuration = 0;
			double sumExpiredDemandsPriceSquaredDiff = 0;
			double priceStdDevExpiredDemands = 0;

			int expired_demands_quantity = 0;
			int count_expired_demands = 0;

			// Count the average price and duration of expired demands
			for (auto &tx : m_expired_demands.GetTransactions(-1))
			{
				averageExpiredDemandsPrice += tx->GetPrice();
				averageExpiredDemandsDuration += tx->GetDuration();
				expired_demands_quantity += tx->GetQuantity();
				count_expired_demands++;
			}

			averageExpiredDemandsPrice /= count_expired_demands;
			averageExpiredDemandsDuration /= count_expired_demands;

			// Calculate standard deviation of price for expired demands
			for (auto &tx : m_expired_demands.GetTransactions(-1))
			{
				double priceDiff = tx->GetPrice() - averageExpiredDemandsPrice;
				sumExpiredDemandsPriceSquaredDiff += priceDiff * priceDiff;
			}
			if (count_expired_demands > 1)
			{
				priceStdDevExpiredDemands = sqrt(sumExpiredDemandsPriceSquaredDiff / (count_expired_demands - 1));
			}

			// Repeat the same process for expired offers
			double averageExpiredOffersPrice = 0;
			double averageExpiredOffersDuration = 0;
			double sumExpiredOffersPriceSquaredDiff = 0;
			double priceStdDevExpiredOffers = 0;

			int expired_offers_quantity = 0;
			int count_expired_offers = 0;

			for (auto &tx : m_expired_offers.GetTransactions(-1))
			{
				averageExpiredOffersPrice += tx->GetPrice();
				averageExpiredOffersDuration += tx->GetDuration();
				expired_offers_quantity += tx->GetQuantity();
				count_expired_offers++;
			}
			averageExpiredOffersPrice /= count_expired_offers;
			averageExpiredOffersDuration /= count_expired_offers;

			// Calculate standard deviation of price for expired offers
			for (auto &tx : m_expired_offers.GetTransactions(-1))
			{
				double priceDiff = tx->GetPrice() - averageExpiredOffersPrice;
				sumExpiredOffersPriceSquaredDiff += priceDiff * priceDiff;
			}
			if (count_expired_offers > 1)
			{
				priceStdDevExpiredOffers = sqrt(sumExpiredOffersPriceSquaredDiff / (count_expired_offers - 1));
			}

			double txFinality_tmp = 0;
			double interblock = 0;
			int countTxs_tmp = 0;

			double txFinality_tmp_normal = 0;
			int countTxs_tmp_normal = 0;

			int matched_offers = 0;
			int matched_demands = 0;

			double average_match_demand_delay = 0;
			double average_match_offers_delay = 0;
			double average_total_demand_delay = 0;
			double average_total_offers_delay = 0;

			double total_expired_offers = 0;
			double total_expired_demands = 0;

			int count_offers_quantity = 0;
			int count_demands_quantity = 0;
			double total_matched_quantity = 0;

			int count_nb_matched_transactions = 0;

			double average_price = 0;
			double sum_of_squared_diffs = 0;
			double price_stddev = 0;

			double average_duration = 0;

			int count_attack_txs = 0;

			double average_decay_rate = 0;

			for (auto &block : m_blockchain.GetMainChain())
			{
				// Count the number of blocks where my id is in it
				if (block.GetRsuId() < m_paramerters.malicious)
				{

					count_attack_txs++;
				}

				for (auto &tx : block.GetTransactions())
				{

					txFinality_tmp_normal += (double)(block.GetTimeReceived() - tx->GetTimeCreated());
					countTxs_tmp_normal++;

					interblock += block.GetTimeReceived() - block.GetTimeCreated();
					txFinality_tmp += (double)(block.GetTimeReceived() - tx->GetTimeCreated());
					countTxs_tmp++;

					if (tx->GetType() == TRADE_TYPE::OFFER)
					{
						count_offers_quantity += tx->GetQuantity();
						// cout << "count_offers_quantity : " << tx->GetQuantity() << endl;
					}
					else if (tx->GetType() == TRADE_TYPE::DEMAND)
					{
						count_demands_quantity += tx->GetQuantity();
					}
					else
					{
						cout << "Error " << endl;
						exit(0);
					}
				}

				// Loop through matched transactions and compute the delay between creation to match
				for (auto &t : block.GetMatchedTransactions())
				{
					// cout << "OfferMatchDelay: " << t->GetOfferMatchDelay() << endl;
					// cout << "DemandMatchDelay: " << t->GetDemandMatchDelay() << endl;
					average_match_offers_delay += t->GetOfferMatchDelay();
					average_match_demand_delay += t->GetDemandMatchDelay();
					average_total_offers_delay += t->GetOfferTotalDelay();
					average_total_demand_delay += t->GetDemandTotalDelay();
					total_matched_quantity += t->GetQuantity();
					count_nb_matched_transactions++;
					average_price += t->GetPrice();

					average_decay_rate += (block.GetTimeReceived()) / (double)m_paramerters.duration;

					// average_duration += t->GetDuration(),
					// cout << "TX : " << t->GetOfferTx() << ", " << t->GetDemandTx() << " :: " << t->GetQuantity() << endl;
				}

				// total_expired_demands += block.GetExpiredDemandsQuantity();
				// total_expired_offers += block.GetExpiredOffersQuantity();
				// cout << "(" << total_expired_demands << "," << total_expired_offers << ")" << endl;
			}

			cout << endl;
			cout << "************Results***************" << endl;
			cout << endl;
			if (count_nb_matched_transactions > 0)
			{
				average_decay_rate /= (double)count_nb_matched_transactions;

				average_match_offers_delay /= (double)count_nb_matched_transactions;
				average_match_demand_delay /= (double)count_nb_matched_transactions;
				average_total_offers_delay /= (double)count_nb_matched_transactions;
				average_total_demand_delay /= (double)count_nb_matched_transactions;
				average_price /= (double)count_nb_matched_transactions;

				// average_duration /= (double)count_nb_matched_transactions;

				// Calculate the sum of squared differences from the mean
				for (auto &block : m_blockchain.GetMainChain())
				{
					for (auto &t : block.GetMatchedTransactions())
					{
						double diff = t->GetPrice() - average_price;
						sum_of_squared_diffs += diff * diff;
					}
				}
				// Calculate the variance
				double variance = sum_of_squared_diffs / count_nb_matched_transactions;

				// Calculate the standard deviation
				price_stddev = sqrt(variance);
			}

			/*****Latency*****/
			cout << " -->Offer total delay : " << average_total_offers_delay << endl;
			cout << " -->Offer matching delay : " << average_match_offers_delay << endl;

			cout << " -->Demand total delay : " << average_total_demand_delay << endl;
			cout << " -->Demand matching delay : " << average_match_demand_delay << endl;

			cout << " -->Blokchain delay : " << txFinality_tmp_normal / (double)countTxs_tmp_normal << endl;
			/****************/
			cout << endl;

			/*****Ressources utilization*****/
			cout << " -->% Offers : " << count_offers_quantity / (double)(count_offers_quantity + count_demands_quantity) << endl;
			cout << " -->% Demands : " << count_demands_quantity / (double)(count_demands_quantity + count_offers_quantity) << endl;
			cout << " -->% Wasted ressources : " << ((expired_offers_quantity) / (double)count_offers_quantity) << endl;

			cout << endl;
			/*****User utilization*****/
			// cout << "talal mateched quantities: " << count_demands_quantity << " total_matched_quantity: " << total_matched_quantity << endl;
			// cout << "total_expired_demands: " << total_expired_demands << " expired_demands_quantity: " << expired_demands_quantity << endl;
			cout << " -->% User statisfaction : " << ((total_matched_quantity / (double)count_demands_quantity) + (total_matched_quantity / (double)count_offers_quantity)) / 2 << endl;

			cout << endl;

			cout << " -->Average expired demands quantity : " << expired_demands_quantity / (double)count_expired_demands << " STDV: " << price_stddev << endl;

			/************/
			cout << endl;

			/******Price Volatility******/
			cout << " -->Average price : " << average_price << " STDV: " << price_stddev << endl;
			cout << " -->Average decay price : " << average_decay_rate << endl;

			cout << endl;

			double nsp_profit = (average_price * m_paramerters.commission / (double)100);

			cout << " -->Seller savings : " << average_price - m_c1 - m_c2 << endl;
			cout << " -->Buyer savings : " << m_paramerters.INITIAL_PRICE * (1 - average_decay_rate) - average_price << endl;
			cout << " -->NSP profit : " << nsp_profit << endl;

			cout << endl;

			cout << "--->Expired demands price : " << averageExpiredDemandsPrice << " STDV: " << priceStdDevExpiredDemands << endl;
			cout << "--->Expired offers price : " << averageExpiredOffersPrice << " STDV: " << priceStdDevExpiredOffers << endl;

			cout << endl;

			// cout << " -->Average price : " << average_price << " STDV: " << price_stddev << endl;

			cout << "--->Expired demands duration: " << averageExpiredDemandsDuration << endl;
			cout << "--->Expired offers duration: " << averageExpiredOffersDuration << endl;

			cout << endl;

			cout << " -->NB Generate Transactions : " << countTxs_tmp << endl;
			cout << " -->NB Matched Transactions : " << count_nb_matched_transactions << endl;

			cout << " -->Matched quantity :" << total_matched_quantity << endl;

			cout << " -->Offers quantity :" << count_offers_quantity << endl;
			cout << " -->Demands quantity :" << count_demands_quantity << endl;

			cout << "Front-running attacking rate : " << count_attack_txs / double(countTxs_tmp) << endl;

			// cout << " -->Latency : " << txFinality_tmp / (double)countTxs_tmp << endl;
			// cout << " -->Interblock : " << interblock / (double)countTxs_tmp << endl;
			// cout << " -->Mem Size : " << m_mempool.GetNumberOfTxs() << endl;
			cout << endl;
			cout << "*******************************" << endl;

			ofstream outputFile;
			outputFile.open("../results/latency.csv", ios::app);

			cout << "Average Decay: " << average_decay_rate << endl;

			auto buyer_profit = m_paramerters.INITIAL_PRICE * (1 - average_decay_rate) - average_price;
			buyer_profit = max(buyer_profit, 0.0);

			if (outputFile)
			{
				outputFile << m_paramerters.nb_robots
						   << ", " << m_paramerters.scenario_type
						   << ", " << m_paramerters.seed
						   << ", " << m_paramerters.duration

						   << ", " << average_total_offers_delay
						   << ", " << average_total_demand_delay
						   << ", " << txFinality_tmp_normal / (double)countTxs_tmp_normal

						   << ", " << count_offers_quantity / (double)(count_offers_quantity + count_demands_quantity)
						   << ", " << count_demands_quantity / (double)(count_demands_quantity + count_offers_quantity)
						   << ", " << ((expired_offers_quantity) / (double)count_offers_quantity)

						   << ", " << total_matched_quantity / (count_demands_quantity + count_offers_quantity)

						   << ", " << m_paramerters.malicious
						   << ", " << count_attack_txs / double(countTxs_tmp)

						   << ", " << m_paramerters.commission
						   << ", " << average_price - m_c1 - m_c2
						   << ", " << buyer_profit 
						   << ", " << nsp_profit
						   << ", " << average_price
						   << endl;
				// - nsp_profit

				//* (1 - average_decay_rate)
			}

			outputFile.close();
			cout << " Offers:  " << count_offers << " : "
				 << " Demands : " << count_demands << endl;
		}

		Application::DoDispose();
	}

	// Function to calculate and log resource utilization
	void Robot::logResourceUtilization()
	{
		// double resourceUtilization = calculateResourceUtilization();
		// cout << "Timestamp: " << GetSeconds() << " Resource Utilization: " << resourceUtilization << endl;
	}

	// Function to schedule the logging event
	void Robot::scheduleLoggingEvent()
	{
		// for (int i = 0; i < m_paramerters.duration; ++i)
		// {
		// 	Simulator::Schedule(Seconds(i), &logResourceUtilization);
		// }
	}

	void Robot::StartApplication(void)
	{
		m_robotId = GetNode()->GetId();
		m_generator.seed(m_paramerters.seed + m_robotId);

		if (m_robotId < m_paramerters.nb_robots)
		{
			m_malicious = true;
		}

		for (uint32_t i = 0; i < m_robots.GetN(); i++)
		{
			m_robotAddresses.push_back(GetIp4Address(i));
		}

		if (!m_socket)
		{
			m_socket = Socket::CreateSocket(GetNode(), TypeId::LookupByName("ns3::TcpSocketFactory"));
			// Ipv4Address::GetAny()
			InetSocketAddress local = InetSocketAddress(GetIp4Address(m_robotId), m_port);
			m_socket->Bind(local);
			m_socket->SetAllowBroadcast(false);
			m_socket->Listen();
			m_socket->ShutdownSend();
		}

		m_socket->SetRecvCallback(MakeCallback(&Robot::HandleRead, this));
		m_socket->SetAcceptCallback(
			MakeNullCallback<bool, Ptr<Socket>, const Address &>(),
			MakeCallback(&Robot::HandleAccept, this));
		m_socket->SetCloseCallbacks(
			MakeCallback(&Robot::HandlePeerClose, this),
			MakeCallback(&Robot::HandlePeerError, this));

		// Ptr<Socket> socket = Socket::CreateSocket(GetNode(), TcpSocketFactory::GetTypeId());
		// socket->Connect(InetSocketAddress(ip4_address, m_port));

		// auto m_peersAddresses = m_rsusAddresses[m_rsuId];
		for (auto &a : m_robotAddresses)

		{
			m_peersSockets[a] = Socket::CreateSocket(GetNode(), TcpSocketFactory::GetTypeId());
			m_peersSockets[a]->Connect(InetSocketAddress(a, m_port));
			// m_peersSockets[*i]->SetSendCallback(MakeCallback(&Rsu::WriteUntilBufferFull, this));
		}

		// cout << "start robot " << m_robotId << " At " << GetSeconds() << " Adress : " << GetIp4Address(m_robotId) << endl;
		// // GenerateHello();
		// // Simulator::Schedule(Seconds(10), &Robot::GenerateHello, this);
		// auto hello = Hello(GetSeconds(), m_robotId);

		// auto pcket_type = MyPacket::HELLO;

		// string msg = hello.Serialize();

		// string msg_sig = "sig";
		// auto packet = MyPacket(pcket_type, msg, m_robotId,
		// 					   msg_sig, msg.size(), GetSeconds());

		// Simulator::Schedule(Seconds(1), &Robot::Broadcast, packet);
		// Send(packet, m_robotId);
		GenerateTransaction();

		// Start consensus
		m_tendermint = Tendermint();

		m_tendermint.SetStartTime(GetSeconds());

		map<uint32_t, float> powers = {};
		for (auto i = 0; i < m_robots.GetN(); ++i)
		{
			powers[i] = 1;
		}
		m_tendermint.SetValidators(powers);

		// auto boundFunction = bind(&Robot::MatchingLogic, m_unmatched_demands, m_unmatched_offers);
		// Deploy smart contract

		auto contract = SmartContract(SMARTCONTRACTS::MATCHING, bind(&Robot::MatchingLogic, m_tendermint.GetProposeBlock(), this));

		m_blockchain.DeploySmartContract(contract);

		// If I'm the leader the propose a block
		if (IsTendermintLeader())
		{

			Simulator::Schedule(Seconds(0), &Robot::SendPropose, this);
		}
		else
		{
			// scheduledTendermintNilVoteEvent = Simulator::Schedule(Seconds(0 + timeout_propose + m_tendermint.GetRound() * timeout_propose_delta), &Robot::SendNilPrevote, this);
		}
	}

	void Robot::SendNilPrevote(void)
	{
		m_tendermint.Init();
		m_tendermint.NewRound();
		cout << m_robotId << " sends nilprevote " << endl;
		auto pcket_type = MyPacket::NIL_PREVOTE;
		auto nilprevote = PrevoteCommitNil(m_robotId, m_blockchain.GetHeight() + 1, m_tendermint.GetRound());
		string msg = nilprevote.Serialise();
		string msg_sig = "";
		auto packet = MyPacket(pcket_type, msg, m_robotId,
							   msg_sig, msg.size(), GetSeconds());

		// Broadcast prevote
		Broadcast(packet);
	}

	void Robot::SendNilPrecommit(void)
	{
		cout << m_robotId << " sends nilprecommit " << endl;
		auto pcket_type = MyPacket::NIL_PRECOMMIT;
		auto nilprecommit = PrevoteCommitNil(m_robotId, m_blockchain.GetHeight() + 1, m_tendermint.GetRound());
		string msg = nilprecommit.Serialise();
		string msg_sig = "";
		auto packet = MyPacket(pcket_type, msg, m_robotId,
							   msg_sig, msg.size(), GetSeconds());

		// Broadcast prevote
		Broadcast(packet);
	}

	void Robot::IncreaseResources(const int quality)
	{
		m_resources += quality;
	}
	void Robot::DecreaseResources(const int quality)
	{
		m_resources -= quality;
	}

	void Robot::printQueue(const std::priority_queue<std::shared_ptr<Transaction>,
													 std::vector<std::shared_ptr<Transaction>>,
													 ns3::TransactionComparator> &q)
	{
		auto copy = q; // Make a copy of the queue to preserve the original
		while (!copy.empty())
		{
			std::shared_ptr<Transaction> element = copy.top();
			std::cout << "(" << element->GetTxHash() << "," << element->GetQuantity() << ")"
					  << " ";
			copy.pop();
		}
		std::cout << std::endl;
	}

	void Robot::MatchingLogic(Block *proposed_block, Robot *robot)
	{
		// Temporary queues to hold unmatched or valid transactions
		queue<shared_ptr<Transaction>> tempDemands, tempOffers;

		// Loop while there are demands and offers in the queues
		while (!robot->m_unmatched_demands.empty() && !robot->m_unmatched_offers.empty())
		{
			// Fetch the top transactions from the demands and offers queues
			auto demand = robot->m_unmatched_demands.top();
			robot->m_unmatched_demands.pop();

			auto offer = robot->m_unmatched_offers.top();
			robot->m_unmatched_offers.pop();

			bool demandExpired = demand->IsExpired(GetSeconds());
			bool offerExpired = offer->IsExpired(GetSeconds());

			if (demandExpired || offerExpired)
			{
				if (demandExpired)
				{
					// robot->m_expired_demands.AddTransaction(demand);
					// Additional logic for expired demand
				}
				if (offerExpired)
				{
					// robot->m_expired_offers.AddTransaction(offer);
					// Additional logic for expired offer
					if (robot->m_robotId == offer->GetSourceId())
					{
						robot->IncreaseResources(offer->GetQuantity());
					}
				}

				// If only one of the transactions is expired, re-queue the other for future processing
				if (!demandExpired)
				{
					tempDemands.push(demand);
				}
				if (!offerExpired)
				{
					tempOffers.push(offer);
				}
			}
			else if (demand->GetSourceId() != offer->GetSourceId())
			{
				if (demand->GetPrice() >= offer->GetPrice())
				{
					auto demand_quantity = demand->GetQuantity();
					auto offer_quantity = offer->GetQuantity();
					auto match_quantity = min(demand_quantity, offer_quantity);

					// Match found based on price and quantity criteria
					auto matched_transaction = make_shared<MatchedTransaction>();
					matched_transaction->SetOfferTx(offer->GetTxHash());
					matched_transaction->SetDemandTx(demand->GetTxHash());
					matched_transaction->SetQuantity(match_quantity);
					matched_transaction->SetResource(demand->GetResource());
					matched_transaction->SetPrice(offer->GetPrice());

					matched_transaction->SetOfferMatchDelay(GetSeconds() - offer->GetTimeConfirmed());
					matched_transaction->SetDemandMatchDelay(GetSeconds() - demand->GetTimeConfirmed());
					matched_transaction->SetOfferTotalDelay(GetSeconds() - offer->GetTimeCreated());
					matched_transaction->SetDemandTotalDelay(GetSeconds() - demand->GetTimeCreated());

					// Handle remaining quantity for offers and demands
					auto remaining_offer_quantity = offer_quantity - match_quantity;
					auto remaining_demand_quantity = demand_quantity - match_quantity;

					if (remaining_offer_quantity > 0)
					{
						auto new_offer = make_shared<Transaction>(offer->GetTxHash(), offer->GetSourceId(), offer->GetType(), offer->GetResource(), remaining_offer_quantity, offer->GetDuration(), offer->GetPrice());
						// new_offer->SetQuantity(remaining_offer_quantity);
						new_offer->SetTimeCreated(offer->GetTimeCreated());
						new_offer->SetTimeConfirmed(offer->GetTimeConfirmed());
						robot->m_unmatched_offers.push(new_offer);
					}

					if (remaining_demand_quantity > 0)
					{
						auto new_demand = make_shared<Transaction>(demand->GetTxHash(), demand->GetSourceId(), demand->GetType(), demand->GetResource(), remaining_demand_quantity, demand->GetDuration(), demand->GetPrice());
						// new_demand->SetQuantity(remaining_demand_quantity);
						new_demand->SetTimeCreated(demand->GetTimeCreated());
						new_demand->SetTimeConfirmed(demand->GetTimeConfirmed());
						robot->m_unmatched_demands.push(new_demand);
					}

					// Add matched transaction to the proposed block
					proposed_block->AddMatchedTransactions(move(matched_transaction));

					// Update robot's resources if it is the source of the demand
					if (robot->m_robotId == demand->GetSourceId())
					{
						robot->IncreaseResources(match_quantity);
					}
				}
			}
			else
			{
				cout << offer->GetPrice() << " >> " << demand->GetPrice() << endl;
				tempDemands.push(demand);
				tempOffers.push(offer);

				if (demand->GetPrice() > offer->GetPrice())
				{
					break;
				}
			}
		}

		// Re-populate the original queues with the remaining transactions
		while (!tempDemands.empty())
		{
			robot->m_unmatched_demands.push(tempDemands.front());
			tempDemands.pop();
		}
		while (!tempOffers.empty())
		{
			robot->m_unmatched_offers.push(tempOffers.front());
			tempOffers.pop();
		}
	}

	void Robot::SendPropose(void)
	{
		m_tendermint.Init();

		// Generate a new block
		Block block;
		block.SetBlockHeight(m_blockchain.GetHeight() + 1);
		block.SetBlockNonce(0);
		block.SetRsuId(m_robotId);
		block.SetTimeCreated(GetSeconds());

		block.SetBlockPrevHash(m_blockchain.GetCurrentTopBlock()->GetBlockHash());

		block.SetTransactions(m_mempool.GetTransactions(nb_txs_per_block));

		block.SetBlockMerkle(block.CalculateMerkle());
		block.CalculateBlockSize();
		block.SetBlockHash(to_string(block.GetBlockHeight()));

		auto propose = Propose(block, m_robotId);
		m_tendermint.SavePropose(propose);

		auto pcket_type = MyPacket::PROPOSE;
		string msg = propose.Serialise();
		string msg_sig = "";
		const auto packet = MyPacket(pcket_type, msg, m_robotId,
									 msg_sig, msg.size(), GetSeconds());

		Broadcast(packet);

		// cout << GetSeconds() << " *** " << m_robotId << " proposes block" << block.GetBlockHeight() << endl;
	}

	void Robot::SendPrevote(void)
	{
		// cout << m_robotId << " sends prevote " << endl;
		auto pcket_type = MyPacket::PREVOTE;
		auto prevote = PrevoteCommit(m_tendermint.GetPropose().block.GetBlockHash(), m_robotId);
		m_tendermint.SavePrevote(prevote);
		string msg = prevote.Serialise();
		string msg_sig = "";
		auto packet = MyPacket(pcket_type, msg, m_robotId,
							   msg_sig, msg.size(), GetSeconds());

		// Broadcast prevote
		Broadcast(packet);
	}

	void Robot::SendPrecommit(void)
	{
		// cout << m_robotId << " sends precommit " << endl;

		auto pcket_type = MyPacket::PRECOMMIT;
		auto precommit = PrevoteCommit(m_tendermint.GetPropose().block.GetBlockHash(), m_robotId);
		m_tendermint.SavePrecommit(precommit);
		string msg = precommit.Serialise();
		string msg_sig = "";
		// unique_ptr<LargeObject> pLarge(new LargeObject());
		MyPacket packet = MyPacket(pcket_type, msg, m_robotId,
								   msg_sig, msg.size(), GetSeconds());

		Broadcast(packet);
	}

	bool Robot::IsTendermintLeader(void)
	{

		return m_robotId == (m_tendermint.GetRound() + m_blockchain.GetHeight()) % m_robots.GetN();
	}

	void Robot::InvokeSmartContract(Block *block)
	{
		m_blockchain.InvokeSmartContract(SMARTCONTRACTS::MATCHING);
		// cout << block->GetBlockHeight() << "**During adding : " << block->GetTimeReceived() << endl;
		m_blockchain.AddBlock(*block);
	}

	void Robot::Read(const string &token)
	{

		// cout << "robot " << m_robotId << " At " << GetSeconds() << " " << token << endl;
		// return;
		auto packet = MyPacket(token);

		if (packet.GetPacketType() == MyPacket::TX)
		{
			// cout << "Tx received : " << packet.GetData() << endl;

			auto tx = make_unique<Transaction>(packet.GetData());

			// Validate transaction:

			// Then save the transaction
			m_mempool.AddTransaction(move(tx));
		}
		else if (packet.GetPacketType() == MyPacket::PROPOSE)
		{
			// Start time
			auto start = std::chrono::high_resolution_clock::now();

			// Cancel the scheduled Event
			Simulator::Cancel(scheduledTendermintNilVoteEvent);
			m_tendermint.Init();

			// Validate & Save the propose message
			auto propose = Propose(packet.GetData());

			// cout << m_robotId << " received PROPOSE of " << propose.block.GetBlockHeight() << " From " << packet.GetSource() << " At" << GetSeconds() << endl;
			// Validate the block and the proposer id

			m_tendermint.SavePropose(propose);

			auto end = std::chrono::high_resolution_clock::now();

			// Calculate elapsed time
			std::chrono::duration<double> elapsed = end - start;

			// Output the elapsed time in seconds
			// std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;

			// Broadcast a prevote message
			Simulator::Schedule(Seconds(sign_delay + verify_delay), &Robot::SendPrevote, this);
		}
		else if (packet.GetPacketType() == MyPacket::PREVOTE)
		{
			// cout << m_robotId << " received PREVOTE from robot " << packet.GetSource() << endl;

			// Save Prevote
			auto prevote = PrevoteCommit(packet.GetData());
			auto is_prevoted = m_tendermint.isPrevoted();
			m_tendermint.SavePrevote(prevote);

			// Check 2/3 voting power
			if (is_prevoted == false && m_tendermint.isPrevoted())
			{
				// cout << m_robotId << " I'm prevoted  " << endl;

				// Wait for prevote timeout before broadcasting
				Simulator::Schedule(Seconds(verify_delay + sign_delay + timeout_prevote + m_paramerters.nb_robots * timeout_prevote_delta), &Robot::SendPrecommit, this);
			}
		}
		else if (packet.GetPacketType() == MyPacket::PRECOMMIT)
		{

			// cout << m_robotId << " received PRECOMMIT from robot " << packet.GetSource() << endl;

			// Save Prevote
			auto precommit = PrevoteCommit(packet.GetData());
			auto is_commited = m_tendermint.isPrecommited();
			m_tendermint.SavePrecommit(precommit);

			// double expired_demands = m_unmatched_demands.RemoveExpiredTransactions(GetSeconds());
			// double expired_offers = m_unmatched_offers.RemoveExpiredTransactions(GetSeconds());

			// Check 2/3 precommits
			if (is_commited == false && m_tendermint.isPrecommited() && m_tendermint.isPrevoted())
			{
				auto block = m_tendermint.GetProposeBlock();
				block->SetTimeReceived(GetSeconds() + timeout_precommit);

				// cout << block->GetBlockHeight() << " Precommited : " << m_tendermint.isPrecommited() << endl;
				// cout << m_robotId << " I'm confirmed " << block->GetBlockHeight() << " At: " << GetSeconds() << endl;

				// cout << "(" << expired_demands << "," << expired_offers << ")" << endl;

				for (const auto &record : block->GetTransactions())
				{
					// Consider the transactions to be confirmed
					record->SetTimeConfirmed(block->GetTimeReceived());
					// cout << " record time confirmed : " << record->GetTimeConfirmed() << endl;
					if (record->GetType() == TRADE_TYPE::DEMAND)
					{
						m_unmatched_demands.push(record);
					}
					else if (record->GetType() == TRADE_TYPE::OFFER)
					{
						m_unmatched_offers.push(record);
					}
					else
					{
						cout << "Error unknow transaction received" << endl;
						exit(0);
					}
				}

				Simulator::Schedule(Seconds(verify_delay + timeout_precommit), &Robot::InvokeSmartContract, this, block);

				// m_blockchain.AddBlock(*block);
				if (block->GetTransactions().size() > 0)
				{
					// cout << "mem before : " << m_mempool.GetNumberOfTxs() << " time : " << GetSeconds() << endl;
					// remove transactions from the mempool
					m_mempool.RemoveTransaction(*block);

					// if (m_blockchain.NbConfirmedTransactions() >= m_paramerters.number_of_txs * m_robots.GetN()) // If there is no more transactions
					// {
					// 	cout << " You have to orginize the transactions: " << GetSeconds() << endl;
					// 	m_unmatched_demands.SortOrginizeTransactions();
					// 	m_unmatched_offers.SortOrginizeTransactions();
					// 	return;
					// }
					// cout << "*************Block********** " << endl;
					// for (auto &t : block->GetTransactions())
					// {
					// 	cout << m_robotId << "Height : " << block->GetBlockHeight() << " Source:: " << block->GetRsuId() << "Received Time: " << block->GetTimeReceived() << " --> " << t->GetTxHash() << endl;
					// }
					// cout << "************************* " << endl;
				}

				// Wait for prevote timeout before broadcasting
				if (IsTendermintLeader())
				{
					// cout << m_robotId << " I'm the leader : " << endl;
					Simulator::Schedule(Seconds(timeout_commit + timeout_precommit + m_paramerters.nb_robots * timeout_precommit_delta),
										&Robot::SendPropose, this);
				}
				else
				{
					// scheduledTendermintNilVoteEvent = Simulator::Schedule(
					// 	Seconds(timeout_commit + timeout_precommit +
					// 			m_tendermint.GetRound() * timeout_precommit_delta +
					// 			timeout_propose + m_tendermint.GetRound() * timeout_propose_delta),
					// 	&Robot::SendNilPrevote, this);
				}
			}
		}
		else if (packet.GetPacketType() == MyPacket::NIL_PREVOTE)
		{
			cout << m_robotId << " received NIL_PREVOTE from robot " << packet.GetSource() << endl;

			// Save Prevote
			auto nilprevote = PrevoteCommitNil(packet.GetData());
			auto is_nilprevoted = m_tendermint.isNilPrevoted();
			m_tendermint.SaveNilPrevote(nilprevote);

			// Check 2/3 voting power
			if (is_nilprevoted == false && m_tendermint.isNilPrevoted())
			{
				// cout << m_robotId << " I'm prevoted  " << endl;

				// Wait for prevote timeout before broadcasting
				Simulator::Schedule(Seconds(timeout_prevote + m_tendermint.GetRound() * timeout_prevote_delta), &Robot::SendNilPrecommit, this);
			}
		}
		else if (packet.GetPacketType() == MyPacket::NIL_PRECOMMIT)
		{
			cout << m_robotId << " received NIL_PRECOMMIT from robot " << packet.GetSource() << endl;
			// Save Prevote
			auto nilprecommit = PrevoteCommitNil(packet.GetData());
			auto is_nilcommited = m_tendermint.isNilPrecommited();
			m_tendermint.SaveNilPrecommit(nilprecommit);

			// Check 2/3 precommits
			if (is_nilcommited == false && m_tendermint.isNilPrecommited() && m_tendermint.isNilPrevoted())
			{
				cout << m_robotId << " I'm nil commited  " << endl;
				// Add block to the blockchain
				// cout << "block height" << m_tendermint.GetPropose().block.GetBlockHeight() << m_tendermint.GetPropose().block << endl;
				auto block = m_tendermint.GetPropose().block;
				block.SetTimeReceived(GetSeconds() + timeout_precommit);

				m_blockchain.AddBlock(block);

				// if (m_blockchain.NbConfirmedTransactions() >= m_paramerters.number_of_txs * m_robots.GetN()) // If there is no more transactions
				// {
				// 	// cout << "I'm here" << endl;
				// 	// exit(0);
				// 	return;
				// }

				// Wait for prevote timeout before broadcasting
				if (IsTendermintLeader())
				{
					cout << m_robotId << " I'm the leader : " << endl;
					Simulator::Schedule(Seconds(timeout_commit + timeout_precommit + m_tendermint.GetRound() * timeout_precommit_delta),
										&Robot::SendPropose, this);
				}
				else
				{
					scheduledTendermintNilVoteEvent = Simulator::Schedule(
						Seconds(timeout_commit + timeout_precommit +
								m_tendermint.GetRound() * timeout_precommit_delta +
								timeout_propose + m_tendermint.GetRound() * timeout_propose_delta),
						&Robot::SendNilPrevote, this);
				}
			}
		}
		else
		{
			cout << "ERROR" << endl;
			return;
		}
	}

	void Robot::HandleRead(Ptr<Socket> socket)
	{
		Ptr<Packet> packet;
		Address from;
		while ((packet = socket->RecvFrom(from)))
		{
			if (packet->GetSize() == 0)
			{ // EOF
				break;
			}
			if (InetSocketAddress::IsMatchingType(from))
			{
				InetSocketAddress iaddr = InetSocketAddress::ConvertFrom(from);
				Ipv4Address source_ipv4 = iaddr.GetIpv4();

				string delimiter = "#";
				size_t pos = 0;
				string parsedPacket;
				char *packetInfo = new char[packet->GetSize() + 1];
				packet->CopyData(reinterpret_cast<uint8_t *>(packetInfo), packet->GetSize());
				packetInfo[packet->GetSize()] = '\0'; // ensure that it is null terminated to avoid bugs

				/**
				 * Add the buffered data to complete the packet
				 */
				ostringstream totalStream;
				totalStream << m_bufferedData[from] << packetInfo;
				string totalReceivedData(totalStream.str());

				while ((pos = totalReceivedData.find(delimiter)) != string::npos)
				{
					parsedPacket = totalReceivedData.substr(0, pos);

					rapidjson::Document d;
					d.Parse(parsedPacket.c_str());

					if (!d.IsObject())
					{
						totalReceivedData.erase(0, pos + delimiter.length());
						continue;
					}
					else
					{
						totalReceivedData.erase(0, pos + delimiter.length());
						// cout << "Received " << parsedPacket << endl;
						// Simulator::Schedule(Seconds(sign_delay + verify_delay), &Robot::Read, this, parsedPacket);
						Read(parsedPacket);
					}
				}
				m_bufferedData[from] = totalReceivedData;
				delete[] packetInfo;
			}
		}
	}

	void Robot::HandleAccept(Ptr<Socket> s, const Address &from)
	{
		s->SetRecvCallback(MakeCallback(&Robot::HandleRead, this));
	}

	void Robot::HandlePeerError(Ptr<Socket> socket)
	{
		// cout << m_robotId << " : HandlePeerClose " << endl;
	}

	void Robot::HandlePeerClose(Ptr<Socket> socket)
	{
		// cout << m_robotId << " : HandlePeerClose " << endl;
	}

	void Robot::StopApplication(void)
	{
		// cout << m_robotId << " : Stop " << endl;
	}

	string Robot::Sign(const string &msg) const
	{
		// TODO

		return "sig";
	}

	void Robot::Broadcast(const MyPacket &p_packet)
	{
		// cout << "robot " << m_robotId << " broadcast " << p_packet.GetData() << " at  " << GetSeconds() << endl;
		for (auto i = 0; i < m_robots.GetN(); i++)
		{
			if (i != m_robotId)
			{
				// cout << "Send packet " << i << " " << endl;
				Send(p_packet, i);
			}
		}

		// Check if packet is saved in the heap then remove it
		// for (auto it = scheduledPackets.begin(); it != scheduledPackets.end(); ++it)
		// {
		// 	if (**it == p_packet)
		// 	{
		// 		scheduledPackets.erase(remove(scheduledPackets.begin(), scheduledPackets.end(), *it));
		// 		delete *it;
		// 	}
		// }
	}

	void Robot::Multicast(const vector<uint32_t> &p_receivers, const MyPacket &p_packet)
	{
		for (const auto &a : p_receivers)
		{
			Send(p_packet, a);
		}
	}

	void Robot::Send(const MyPacket &p_packet, const uint32_t &p_id)
	{
		// Ipv4Address ip4_address = GetIp4Address(p_id);

		Ipv4Address ip4_address = GetIp4Address(p_id);

		Ptr<Socket> socket;

		auto it = m_peersSockets.find(ip4_address);
		if (it == m_peersSockets.end())
		{
			// Create the socket if it doesn't exist
			m_peersSockets[ip4_address] = Socket::CreateSocket(GetNode(), TcpSocketFactory::GetTypeId());
			m_peersSockets[ip4_address]->Connect(InetSocketAddress(ip4_address, m_port));
		}
		socket = m_peersSockets[ip4_address];

		// Ptr<Socket> socket = Socket::CreateSocket(GetNode(), TcpSocketFactory::GetTypeId());
		// socket->Connect(InetSocketAddress(ip4_address, m_port));
		string buffer = p_packet.Serialize() + "#";
		m_data[socket] += buffer;

		socket->SetSendCallback(MakeCallback(&Robot::WriteUntilBufferFull, this));
		WriteUntilBufferFull(socket, socket->GetTxAvailable());
	}

	void Robot::WriteUntilBufferFull(Ptr<Socket> sockt, uint32_t txSpace)
	{

		int size = m_data[sockt].size();
		if (size <= 0)
			return;

		int packet_size;

		while (m_data[sockt].size() > 0 && sockt->GetTxAvailable() > 0)
		{
			packet_size = min(size, (int)sockt->GetTxAvailable());
			// packet_size = min(tcpSegSize, packet_size);
			int error = sockt->Send(reinterpret_cast<const uint8_t *>(m_data[sockt].substr(0, packet_size).c_str()), packet_size, 0);

			if (error < 0)
				return;

			m_data[sockt].erase(0, error);
		}

		if (m_data[sockt].size() == 0)
			m_data.erase(sockt);
	}

	Ipv4Address Robot::GetIp4Address(const uint32_t &p_id)
	{
		Ptr<Ipv4> ipv4 = m_robots.Get(p_id)->GetObject<Ipv4>();
		Ipv4Address addr = ipv4->GetAddress(1, 0).GetLocal();
		return addr;
	}

} // namespace ns3
  //
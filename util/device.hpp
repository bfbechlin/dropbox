#ifndef __DEVICE_HPP__
#define __DEVICE_HPP__


class Device
{
	private:
		Communication activeChannel;
		Communication passiveChannel;
		std::queue<Action*> actions;
		FolderManager* folder;
	public:
		Device(void);
		Device(Communication activeChannel, Communication passiveChannel, FolderManager *folder);

		void initializeActive();
		void initializePassive();

		void synchronizeActive();
		void synchronizePassive();

		void notifyActive();
		void notifyPassive();

		void uploadActive();
		void uploadPassive();

		void downloadActive();
		void downloadPassive();
};

#endif

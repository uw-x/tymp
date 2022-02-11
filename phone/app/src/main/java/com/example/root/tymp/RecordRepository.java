package com.example.root.tymp;

import android.content.Context;

import java.util.List;

class RecordRepository {

    private RecordDao mRecordDao;
    private List<Record> mAllRecords;

    // Note that in order to unit test the WordRepository, you have to remove the Application
    // dependency. This adds complexity and much more code, and this sample is not about testing.
    // See the BasicSample in the android-architecture-components repository at
    // https://github.com/googlesamples
    RecordRepository(Context application) {

        RecordRoomDatabase db = RecordRoomDatabase.getDatabase(application);
        mRecordDao = db.recordDao();
        mAllRecords = mRecordDao.getSortedRecords();

    }

    // Room executes all queries on a separate thread.
    // Observed LiveData will notify the observer when the data has changed.
    List<Record> getAllRecords() {


        return mAllRecords;
    }

    // You must call this on a non-UI thread or your app will throw an exception. Room ensures
    // that you're not doing any long running operations on the main thread, blocking the UI.
    void insert(Record record) {

        RecordRoomDatabase.databaseWriteExecutor.execute(() -> {
            mRecordDao.insert(record);
        });

    }
}
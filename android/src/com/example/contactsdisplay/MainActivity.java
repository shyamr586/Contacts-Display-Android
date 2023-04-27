package com.example.contactsdisplay;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.ContentProviderOperation;
import android.content.ContentResolver;
import android.content.pm.PackageManager;
import android.database.ContentObserver;
import android.database.Cursor;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.provider.ContactsContract;
import android.util.Log;

import org.qtproject.qt.android.bindings.QtActivity;

import java.util.ArrayList;

public class MainActivity extends QtActivity {
    private ContactsObserver contactsObserver;
    public long pointer;
    public ArrayList<String> initialContacts;
    public ArrayList<String> newContacts;
    private static final String[] REQUIRED_PERMISSIONS = {
            Manifest.permission.WRITE_CONTACTS,
            Manifest.permission.READ_CONTACTS
    };
    private static final int PERMISSIONS_REQUEST_CODE = 1;

    public native void setQStringList(long pointer, ArrayList<String> contacts);
    public native void addToModel(long pointer, String element, int index);
    public native void removeFromModel(long pointer, int index);

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        if (hasAllPermissions()) {
            Log.d("ALL PERMISSIONS ALREADY GRANTED? ", "YES");
            //getContacts(true);
            contactsObserver = new ContactsObserver(new Handler());
            getContentResolver().registerContentObserver(ContactsContract.Contacts.CONTENT_URI, true, contactsObserver);
        } else {
            requestPermissions();
        }

    }

    public void setPointer(long nativePointer) {
        pointer = nativePointer;
    }

    private boolean hasAllPermissions() {
        for (String permission : REQUIRED_PERMISSIONS) {
            if (checkSelfPermission(permission) != PackageManager.PERMISSION_GRANTED) {
                return false;
            }
        }
        return true;
    }

    private void requestPermissions() {
        requestPermissions(REQUIRED_PERMISSIONS, PERMISSIONS_REQUEST_CODE);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,  String[] permissions, int[] grantResults) {
        if (requestCode == PERMISSIONS_REQUEST_CODE) {
            if (hasAllPermissions()) {
                Log.d("PERMISSIONS NOW CHANGED TO GRANTED? ", "YES");
                contactsObserver = new ContactsObserver(new Handler());
                getContentResolver().registerContentObserver(ContactsContract.Contacts.CONTENT_URI, true, contactsObserver);
                //getContacts(true);
                // IMPROVEMENT: call the get contacts stuff here
            }
        }
    }

    private class ContactsObserver extends ContentObserver {
        public ContactsObserver(Handler handler) {
            super(handler);
        }

        @Override
        public void onChange(boolean selfChange) {
            super.onChange(selfChange);
            Log.d("Message from the content observer: ", "Change in contacts detected.");
            getContacts(false);
        }
    }

    // unregister the content observer in onDestroy()
    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (contactsObserver != null) {
            getContentResolver().unregisterContentObserver(contactsObserver);
        }
    }
    // DO TWO DO IN BACKGROUND, ONE FOR GETCONTACTS AND ONE FOR CHECK CONTACT CHANGES.

    public void checkContactChanges(ArrayList<String> newArrList){

        for (int i = 0; i< newArrList.size(); i++) {
            String element = newArrList.get(i);
            if (!initialContacts.contains(element)) {
                Log.d("New element:---> ",element);
                Log.d("New elements index :---> ",i+"");
                addToModel(pointer, element, i);
                initialContacts.add(i,element);
            }
        }

        for (int i = 0; i < initialContacts.size(); i++) {
            String element = initialContacts.get(i);
            if (!newArrList.contains(element)) {
                Log.d("Removed element:---> ",element);
                Log.d("Removed elements index :---> ",i+"");
                removeFromModel(pointer,i);
                initialContacts.remove(i);
            }
        }
    }

    public void addDummyContacts(){
        for (int i = 0; i<1000; i++) {
            String dummyName = "Dummy Name "+(i+1);
            addDummyContact(dummyName);
        }
    }

    public void addDummyContact(String dummyName){
        ArrayList<ContentProviderOperation> operations = new ArrayList<>();

        int contactIndex = operations.size();

        operations.add(ContentProviderOperation.newInsert(ContactsContract.RawContacts.CONTENT_URI)
                        .withValue(ContactsContract.RawContacts.ACCOUNT_TYPE, null)
                        .withValue(ContactsContract.RawContacts.ACCOUNT_NAME, null)
                .build());

        operations.add(ContentProviderOperation
                .newInsert(ContactsContract.Data.CONTENT_URI)
                .withValueBackReference(
                        ContactsContract.Data.RAW_CONTACT_ID,
                        contactIndex)
                .withValue(ContactsContract.Data.MIMETYPE, ContactsContract.CommonDataKinds.Phone.CONTENT_ITEM_TYPE)
                .withValue(ContactsContract.CommonDataKinds.Phone.NUMBER, "1333333334")
                .withValue(ContactsContract.Data.MIMETYPE, ContactsContract.CommonDataKinds.Phone.CONTENT_ITEM_TYPE)
                .withValue(ContactsContract.CommonDataKinds.Phone.TYPE, "1").build());

        // Display name/Contact name
        operations.add(ContentProviderOperation
                .newInsert(ContactsContract.Data.CONTENT_URI)
                .withValueBackReference(ContactsContract.Data.RAW_CONTACT_ID,
                        contactIndex)
                .withValue(ContactsContract.Data.MIMETYPE, ContactsContract.CommonDataKinds.StructuredName.CONTENT_ITEM_TYPE)
                .withValue(ContactsContract.CommonDataKinds.StructuredName.DISPLAY_NAME, dummyName)
                .build());

        try {
            ContentResolver contentResolver = getContentResolver();
            contentResolver.applyBatch(ContactsContract.AUTHORITY, operations);
        } catch (Exception e) {
            Log.d("Exception found in getting content resolver: ", e.toString());
        }
    }

    // do new AsyncTask instead of extending from class



    public void getContacts(boolean initial) {
        @SuppressLint("StaticFieldLeak")
        AsyncTask<Boolean, Void, Void> task= new AsyncTask<Boolean, Void, Void>() {
            public String tag = "From the AsyncTask class";
            @SuppressLint("Range")
            @Override
            protected Void doInBackground(Boolean... params) {
                boolean initial = params[0];
                long tStart = System.currentTimeMillis();
                ArrayList<String> contacts = new ArrayList<>();

                if (checkSelfPermission(Manifest.permission.READ_CONTACTS) != PackageManager.PERMISSION_GRANTED) {
                    Log.d("METHOD OF CHECKING PERMISSION IN GET CONTACTS", "IS RUNNING");
                    // Request the required permission if it hasn't been granted yet
                    requestPermissions(REQUIRED_PERMISSIONS, 1);
                    //return contacts; // Empty, as the user needs to grant permission first
                }
                ContentResolver contentResolver = getContentResolver();
                Cursor cursor = contentResolver.query(ContactsContract.Contacts.CONTENT_URI, null, null, null, ContactsContract.CommonDataKinds.Phone.DISPLAY_NAME + " ASC");

                if (cursor != null && ((Cursor) cursor).getCount() > 0) {
                    while (cursor.moveToNext()) {
                        @SuppressLint("Range") String id = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts._ID));
                        @SuppressLint("Range") String name = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME));

                        if (Integer.parseInt(cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts.HAS_PHONE_NUMBER))) > 0) {
                            Cursor phoneCursor = contentResolver.query(ContactsContract.CommonDataKinds.Phone.CONTENT_URI, null, ContactsContract.CommonDataKinds.Phone.CONTACT_ID + " = ?", new String[]{id}, null);

                            if (phoneCursor != null) {
                                while (phoneCursor.moveToNext()) {
                                    String phoneNumber = phoneCursor.getString(phoneCursor.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER));
                                    contacts.add(name + ": " + phoneNumber);
                                }
                                phoneCursor.close();
                            }
                        }
                    }
                    cursor.close();
                }
                long tEnd = System.currentTimeMillis();
                long tDelta = tEnd - tStart;
                Log.d("Time elapsed in milliseconds for loading contacts: ", tDelta+"");
                if (initial){
                    initialContacts = contacts;
                } else {
                    checkContactChanges(contacts);
                }
                Log.d(tag, "SIZE OF THE CONTACTS ARRAYLIST IS "+contacts.size()+"");
                setQStringList(pointer, contacts);
                //return contacts;
                return null;
            }
        };
        task.execute(initial);
    }

}

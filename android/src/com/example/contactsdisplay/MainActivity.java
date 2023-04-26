package com.example.contactsdisplay;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.ContentProviderOperation;
import android.content.ContentResolver;
import android.content.pm.PackageManager;
import android.database.ContentObserver;
import android.database.Cursor;
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
    private static final String[] REQUIRED_PERMISSIONS = {
            Manifest.permission.WRITE_CONTACTS,
            Manifest.permission.READ_CONTACTS
    };
    private static final int PERMISSIONS_REQUEST_CODE = 1;
    public native void addToModel(long pointer, String element, int index);
    public native void removeFromModel(long pointer, int index);

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        if (hasAllPermissions()) {
            Log.d("ALL PERMISSIONS ALREADY GRANTED? ", "YES");
        } else {
            requestPermissions();
        }
        contactsObserver = new ContactsObserver(new Handler());
        getContentResolver().registerContentObserver(ContactsContract.Contacts.CONTENT_URI, true, contactsObserver);
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
            //long tStart = System.currentTimeMillis();
            Log.d("Message from the content observer: ", "Change in contacts detected.");
            ArrayList<String> newArrList = getContacts(false);
            checkContactChanges(newArrList);
            //long tEnd = System.currentTimeMillis();
            //long tDelta = tEnd - tStart;
            //Log.d("Time elapsed in milliseconds for change: ", tDelta+"");
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

//    public void setInitialArrayList(){
//        initialContacts = getContacts();
//    }

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

    @SuppressLint("Range")
    public ArrayList<String> getContacts(boolean initial) {

        long tStart = System.currentTimeMillis();
        ArrayList<String> contacts = new ArrayList<>();

        if (checkSelfPermission(Manifest.permission.READ_CONTACTS) != PackageManager.PERMISSION_GRANTED) {
            // Request the required permission if it hasn't been granted yet
            requestPermissions(REQUIRED_PERMISSIONS, 1);
            return contacts; // Empty, as the user needs to grant permission first
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
        }
        return contacts;

    }

}

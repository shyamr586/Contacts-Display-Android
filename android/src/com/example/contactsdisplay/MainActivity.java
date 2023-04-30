package com.example.contactsdisplay;

import static java.util.Objects.isNull;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.ContentProviderOperation;
import android.content.ContentResolver;
import android.content.pm.PackageManager;
import android.database.ContentObserver;
import android.database.Cursor;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.provider.ContactsContract;
import android.util.Log;

import androidx.annotation.RequiresApi;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.qtproject.qt.android.bindings.QtActivity;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class MainActivity extends QtActivity {
    private ContactsObserver contactsObserver;
    public long pointer;
    Handler handler;
    private static final String[] REQUIRED_PERMISSIONS = {
            Manifest.permission.WRITE_CONTACTS,
            Manifest.permission.READ_CONTACTS
    };
    private static final int PERMISSIONS_REQUEST_CODE = 1;
    public long lastSyncTime = 0;


    public native void setQStringList(long pointer, String contacts, boolean inital);
    public native void addToModel(long pointer, String contacts);
    public native void removeFromModel(long pointer, String contactId);

    @Override
    public void onCreate(Bundle savedInstanceState) {

        //PUT THE PERMISSIONS IN GET CONTACTS IN THE GET CONTACTS AND THE GET CONTACTS' BODY IS INSIDE THE (HAS ALL PERMISSIONS) CONDITION.
        // THE CONTACTS OBSERVER SHOULDNT RUN EVERYTIME CONTACTSOBSERVER RUNS SO JUST PUT IF CONTACTSOBSERVER === NULL THEN ASSIGN VALUE
        HandlerThread thread = new HandlerThread("MyHandlerThread");
        thread.start();

// creates the handler using the passed looper
        handler = new Handler(thread.getLooper());
        super.onCreate(savedInstanceState);
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
                contactsObserver = new ContactsObserver(handler);
                getContentResolver().registerContentObserver(ContactsContract.Contacts.CONTENT_URI, true, contactsObserver);
                getContacts(true);
            }
        }
    }

    private class ContactsObserver extends ContentObserver {
        public ContactsObserver(Handler handler) {
            super(handler);
        }

        @RequiresApi(api = Build.VERSION_CODES.N)
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
        if (hasAllPermissions()) {
            @SuppressLint("StaticFieldLeak")
            AsyncTask<Boolean, Void, Void> task = new AsyncTask<Boolean, Void, Void>() {
                public String tag = "From the AsyncTask class";
                JSONArray jsonArrContacts = new JSONArray();
                boolean deleteEvent = false;
                int sortId = 0;
                @SuppressLint("Range")
                @Override
                protected Void doInBackground(Boolean... params) {
                    boolean initial = params[0];
                    long tStart = System.currentTimeMillis();
                    ArrayList<String> contacts = new ArrayList<>();

                    if (contactsObserver == null) {
                        contactsObserver = new ContactsObserver(handler);
                        getContentResolver().registerContentObserver(ContactsContract.Contacts.CONTENT_URI, true, contactsObserver);
                    }
                    ContentResolver contentResolver = getContentResolver();
                    String selection = ContactsContract.Contacts.CONTACT_LAST_UPDATED_TIMESTAMP + " > ?";
                    String[] selectionArgs = new String[] {String.valueOf(lastSyncTime)};
                    Cursor cursor1 = contentResolver.query(ContactsContract.Contacts.CONTENT_URI, null, selection, selectionArgs, ContactsContract.CommonDataKinds.Phone.DISPLAY_NAME + " ASC");

                    if (cursor1.getCount() > 0) {
                        while (cursor1.moveToNext()) {
                            @SuppressLint("Range") String id = cursor1.getString(cursor1.getColumnIndex(ContactsContract.Contacts._ID));
                            @SuppressLint("Range") String name = cursor1.getString(cursor1.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME));

                            if (Integer.parseInt(cursor1.getString(cursor1.getColumnIndex(ContactsContract.Contacts.HAS_PHONE_NUMBER))) > 0) {
                                Cursor phoneCursor = contentResolver.query(ContactsContract.CommonDataKinds.Phone.CONTENT_URI, null, ContactsContract.CommonDataKinds.Phone.CONTACT_ID + " = ?", new String[]{id}, null);

                                if (phoneCursor != null) {
                                    while (phoneCursor.moveToNext()) {
                                        String phoneNumber = phoneCursor.getString(phoneCursor.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER));
                                        contacts.add(name + ": " + phoneNumber);
                                        String[] contactDetails = new String[] {name, phoneNumber, String.valueOf(sortId++)};
                                        JSONObject jsonContactObj = new JSONObject();

                                        try {
                                            jsonContactObj.put("contactID", id);
                                            jsonContactObj.put("name", name);
                                            jsonContactObj.put("phoneNumber", phoneNumber);
                                        } catch (JSONException e) {
                                            throw new RuntimeException(e);
                                        }
                                        jsonArrContacts.put(jsonContactObj);
                                    }
                                    phoneCursor.close();
                                }
                            }
                        }
                        cursor1.close();
                    } else {
                        String[] projection = new String[]{ContactsContract.DeletedContacts.CONTACT_ID};
                        selection = ContactsContract.DeletedContacts.CONTACT_DELETED_TIMESTAMP + " > ?";
                        Cursor cursor2 = contentResolver.query(ContactsContract.DeletedContacts.CONTENT_URI, projection, selection, selectionArgs, null);

                        while (cursor2.moveToNext()) {
                            String contactId = cursor2.getString(cursor2.getColumnIndex(ContactsContract.DeletedContacts.CONTACT_ID));
                            // Handle deleted contact here
                            deleteEvent = true;
                            removeFromModel(pointer, contactId);
                        }
                        cursor2.close();

                    }
                    lastSyncTime = System.currentTimeMillis();
                    long tEnd = System.currentTimeMillis();
                    long tDelta = tEnd - tStart;
                    Log.d("Time elapsed in milliseconds for loading contacts: ", tDelta + "");
                    if (!deleteEvent){
                        Log.d("THE UPDATE IS GETTING CALLED", "YES");
                        setQStringList(pointer, jsonArrContacts.toString(), initial);
                    }
                    deleteEvent = false;

                    Log.d(tag, "SIZE OF THE CONTACTS ARRAYLIST IS " + contacts.size() + "");
                    return null;
                }
            };
            task.execute(initial);
        } else {
            requestPermissions();
        }
    }

}

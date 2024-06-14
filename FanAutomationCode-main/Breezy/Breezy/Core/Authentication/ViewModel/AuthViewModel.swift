//
//  AuthViewModel.swift
//  Breezy
//
//  Created by Mohammed Rahil Hussain on 4/10/24.
//

import Foundation
import Firebase
import FirebaseFirestoreSwift

protocol AuthenticationFormProtocol {
    var formIsValid: Bool { get }
}

@MainActor
class AuthViewModel: ObservableObject {
    @Published var userSession: FirebaseAuth.User?
    @Published var currentUser: User?
    @Published var errorMessage: String?
    
    init() {
        self.userSession = Auth.auth().currentUser
        
        Task {
            await fetchUser()
        }
    }
    
    func signIn(withEmail email: String, password: String) async throws {
        do {
            let result = try await Auth.auth().signIn(withEmail: email, password: password)
            self.userSession = result.user
            await fetchUser()
        } catch let error as NSError {
            if error.code == AuthErrorCode.userNotFound.rawValue {
                // Update your UI to display the message
                DispatchQueue.main.async {
                    self.errorMessage = "User does not exist. Please check your email or sign up."
                }
            } else {
                print("DEBUG: Failed to log in with error \(error.localizedDescription)")
            }
        }
    }
    
    func createUser(withEmail email: String, password: String, fullname: String) async throws {
        do {
            let result = try await Auth.auth().createUser(withEmail: email, password: password)
            self.userSession = result.user
            let user = User(id: result.user.uid, fullname: fullname, email: email)
            let encodedUser = try Firestore.Encoder().encode(user)
            try await Firestore.firestore().collection("users").document(user.id).setData(encodedUser)
            await fetchUser()
        } catch {
            print("DEBUG: Failed to create user with error \(error.localizedDescription)")
        }
    }
    
    func signOut() {
        do {
            try Auth.auth().signOut()
            self.userSession = nil
            self.currentUser = nil
        } catch {
            print("DEBUG: Failed to sign out with error \(error)")
        }
    }
    
    func deleteAccount() async throws {
        guard let user = Auth.auth().currentUser else { return }
            
            // Delete the user's data from Firestore
            do {
                try await Firestore.firestore().collection("users").document(user.uid).delete()
            } catch {
                print("DEBUG: Failed to delete user data with error \(error.localizedDescription)")
            }
            
            // Delete the user's account from Firebase Authentication
            do {
                try await user.delete()
                self.userSession = nil
                self.currentUser = nil
            } catch {
                print("DEBUG: Failed to delete user account with error \(error.localizedDescription)")
            }
    }
    
    func fetchUser() async {
        guard let uid = Auth.auth().currentUser?.uid else { return }
        
        guard let snapshot = try? await Firestore.firestore().collection("users").document(uid).getDocument() else { return }
        self.currentUser = try? snapshot.data(as: User.self)
    }
}

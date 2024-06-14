//
//  DataItem.swift
//  Breezy
//
//  Created by Mohammed Rahil Hussain on 2/28/24.
//

import Foundation
import SwiftData

@Model
class DataItem: Identifiable {
    var id: String
    var name: String
    
    init(name: String) {
        id = UUID().uuidString
        self.name = name
    }
}
